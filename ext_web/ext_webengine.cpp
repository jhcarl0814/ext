
#include "ext_webengine.hpp"
#include "ext_webengine_p.hpp"

#include <QCursor>
#include <QDesktopServices>
#include <QMouseEvent>

#include <QApplication>
#include <QToolTip>

#include "../ext_infrastructure/ext_debug.hpp"

void WebEngineView::ctor_impl()
{
    WebEnginePage *view_web_page = new WebEnginePage(this);
    setPage(view_web_page);

    qApp->installEventFilter(this);
    setMouseTracking(true);
    connect(view_web_page, &QWebEnginePage::linkHovered, this, [/*this*/](QString const &url) { /*ext_debug_log((url),qDebug_compact());*/QToolTip::hideText();QToolTip::showText(QCursor::pos(), url); });
}

bool WebEngineView::eventFilter(QObject *object, QEvent *event)
{
    if(object->parent() == this && event->type() == QEvent::Type::MouseMove) // https://forum.qt.io/topic/54233/how-to-capture-mouse-events-in-webengineview
    {
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
    }
    return QWebEngineView::eventFilter(object, event);
}

bool WebEnginePage::acceptNavigationRequest(QUrl const &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    if(isMainFrame && type == QWebEnginePage::NavigationType::NavigationTypeLinkClicked)
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    else
    {
        return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
}
