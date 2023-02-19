#include "ext_image.hpp"

#include <QPainter>

#include "../ext_core/ext_thread.hpp"

QByteArray QImage_to_QByteArray(QImage image)
{
    QByteArray ba;
    {
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG", 100);
    }
    return ba;
}

QIcon placeholder_icon()
{
    return qvariant_cast<QIcon>(qApp->property("placeholder_icon"));
}

std::optional<QIcon> filePath_to_QIcon_widgets(QString filePath)
{
    QFileIconProvider *get_icon_file_icon_provider = static_cast<QFileIconProvider *>(qvariant_cast<void *>(qApp->property("get_icon_file_icon_provider")));
    QIcon icon = get_icon_file_icon_provider->icon(QFileInfo(filePath));
    if(icon.isNull())
        return std::nullopt;
    else
        return icon;
}
std::optional<QIcon> filePath_to_QIcon_gui(QString filePath)
{
    QFileSystemModel *get_icon_file_system_model = qvariant_cast<QFileSystemModel *>(qApp->property("get_icon_file_system_model"));
    QFileInfo file_info(filePath);
    get_icon_file_system_model->setRootPath(file_info.absolutePath());
    QIcon icon = get_icon_file_system_model->fileIcon(get_icon_file_system_model->index(file_info.absoluteFilePath()));
    if(icon.isNull())
        return std::nullopt;
    else
        return icon;
}

QIcon opacity_icon_blocking(QIcon icon)
{
    QIcon result;
    for(QPixmap const &pixmap : icon.availableSizes() | std::views::transform([&](QSize const &size)
                                                            { return icon.pixmap(size); }) |
            std::views::transform([](QPixmap pixmap)
                { return pixmap.toImage(); }) |
            std::views::transform([](QImage image)
                {
                    QImage result(image.size(),QImage::Format::Format_ARGB32);
                    result.fill(QColorConstants::Transparent);
                    {
                        QPainter painter(&result);
                        painter.setOpacity(0.5);
                        painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);
                        painter.drawImage(QPoint(0,0),image);
                    }
                    return result; }) |
            std::views::transform([](QImage image)
                { return QPixmap::fromImage(image); }))
    {
        result.addPixmap(pixmap);
        result.addPixmap(pixmap, QIcon::Mode::Disabled);
    }
    return result;
}

future_t<QIcon> opacity_icon(QIcon icon)
{
    //    ext_debug_log((QThread::currentThreadId()), qDebug_compact());
    QIcon result;
    auto images = icon.availableSizes() | std::views::transform([&](QSize const &size)
                                              { return icon.pixmap(size); }) |
        std::views::transform([](QPixmap pixmap)
            { return pixmap.toImage(); }) |
        std::ranges::to<std::vector>();
    co_await coroutine_dispatch_to_thread_t{retrieve_icon_thread()};
    //    ext_debug_log((QThread::currentThreadId()), qDebug_compact());
    std::ranges::for_each(images, [](QImage &image)
        {
            QImage result(image.size(),QImage::Format::Format_ARGB32);
            result.fill(QColorConstants::Transparent);
            {
                QPainter painter(&result);
                painter.setOpacity(0.5);
                painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);
                painter.drawImage(QPoint(0,0),image);
            }
            image= result; });
    co_await coroutine_dispatch_to_main_thread_t{};
    //    ext_debug_log((QThread::currentThreadId()), qDebug_compact());
    for(QPixmap const &pixmap : images |
            std::views::transform([](QImage image)
                { return QPixmap::fromImage(image); }))
    {
        result.addPixmap(pixmap);
        result.addPixmap(pixmap, QIcon::Mode::Disabled);
    }
    co_return result;
}

QThread *retrieve_icon_thread()
{
    return qvariant_cast<QThread *>(qApp->property("retrieve_icon_thread"));
}
