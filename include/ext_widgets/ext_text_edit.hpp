#ifndef EXT_TEXT_EDIT_H
#define EXT_TEXT_EDIT_H

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTextBlock>
#include <QStyle>
#include <QScrollBar>

#include <ext_infrastructure/ext_type_traits.hpp>
#include <ext_infrastructure/ext_debug.hpp>

void make_plain_text_edit_readonly(QPlainTextEdit *plain_text_edit);

void make_line_edit_readonly(QLineEdit *line_edit);

class PlainTextEdit: public QPlainTextEdit
{
  public:
    template<typename Tuple, std::size_t... Is>
    PlainTextEdit(Tuple &&base_args, std::index_sequence<Is...>)
        : QPlainTextEdit(std::get<Is>(std::forward<Tuple>(base_args))...)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        connect(this, &QPlainTextEdit::textChanged, this, [this]()
            {
            setProperty("ext_sizeHint", QVariant());
            updateGeometry(); });
    }
    template<typename Tuple = std::tuple<>>
    PlainTextEdit(Tuple &&base_args = {})
        : PlainTextEdit(std::forward<Tuple>(base_args), std::make_index_sequence<std::tuple_size_v<remove_lvalue_reference_t<Tuple>>>())
    {}
    virtual QSize sizeHint() const override // https://stackoverflow.com/questions/45028105/get-the-exact-height-of-qtextdocument-in-pixels
    {
        if(property("ext_sizeHint").isValid())
        {
            return qvariant_cast<QSize>(property("sizeHint"));
        }
        else
        {
            QSize s(document()->size().toSize());
            //        ext_debug_log((
            //                          QFontMetrics(font()).lineSpacing(), QFontMetrics(font()).leading(), QFontMetrics(font()).height(),
            //                          QFontMetricsF(font()).lineSpacing(), QFontMetricsF(font()).leading(), QFontMetricsF(font()).height(),
            //                          document()->documentLayout()->documentSize(), toPlainText(),
            //                          rect(), contentsMargins(), contentsRect(), viewportMargins(),
            //                          viewport()->rect(), viewport()->contentsMargins(), viewport()->contentsRect(),
            //                          document()->size(), document()->documentMargin(), s),
            //            qDebug_compact());

            qreal height = 0;
            qreal height_max = std::ceil(QFontMetricsF(font()).lineSpacing()) * 10 + document()->documentMargin() * 1;
            if(document()->size().height() >= 10)
            {
                height = height_max;
            }
            else
            {
                for(QTextBlock block = document()->begin(); block != document()->end(); block = block.next())
                {
                    height += document()->documentLayout()->blockBoundingRect(block).height(); // https://www.qtcentre.org/threads/53183-How-to-get-the-line-height-of-a-QTextBlock
                    //            ext_debug_log((block.blockFormat().lineHeight(), block.blockFormat().lineHeightType(), block.blockFormat().topMargin(), block.blockFormat().bottomMargin(), document()->documentLayout()->blockBoundingRect(block), height), qDebug_compact());
                    if(height > height_max)
                    {
                        height = height_max;
                        break;
                    }
                }
            }

            //        height += contentsMargins().top();
            //        height += contentsMargins().bottom();
            //        height += 2 * style()->pixelMetric(QStyle::PixelMetric::PM_DefaultFrameWidth);
            height += document()->documentMargin() * 2;

            //            if(horizontalScrollBar()->isVisible())
            //                height += horizontalScrollBar()->sizeHint().height();
            if(horizontalScrollBarPolicy() == Qt::ScrollBarPolicy::ScrollBarAlwaysOn)
                height += horizontalScrollBar()->height();

            s.setHeight(std::ceil(height) + 1);
            const_cast<PlainTextEdit *>(this)->setProperty("ext_sizeHint", s);
            return (s);

            //        QTextLayout layout(toPlainText(), font());
            //        layout.beginLayout();
            //        QTextLine line = layout.createLine();
            //        layout.endLayout();
            //        ext_debug_log((line.height()), qDebug_compact());
            //        line.setLeadingIncluded(false);
            //        ext_debug_log((line.height()), qDebug_compact());
            //        line.setLeadingIncluded(true);
            //        ext_debug_log((line.height()), qDebug_compact());
            //        height = (std::min)(height, line.height() * 10);
        }
    }
    virtual QSize minimumSizeHint() const override
    {
        return QSize();
    }

  protected:
    //    virtual bool eventFilter(QObject *object, QEvent *event) override
    //    {
    //        if(object == horizontalScrollBar() && (event->type() == QEvent::Type::Show || event->type() == QEvent::Type::Hide) && !event->spontaneous())
    //        {
    //            setProperty("ext_sizeHint", QVariant());
    //            updateGeometry();
    //        }
    //        return QPlainTextEdit::eventFilter(object, event);
    //    }
    virtual void resizeEvent(QResizeEvent *event) override
    {
        setProperty("ext_sizeHint", QVariant());
        updateGeometry();
        QPlainTextEdit::resizeEvent(event);
    }
};

#endif // EXT_TEXT_EDIT_H
