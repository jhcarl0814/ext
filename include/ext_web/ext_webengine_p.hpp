#ifndef EXT_WEBENGINE_P_H
#define EXT_WEBENGINE_P_H

#include <QWebEnginePage>

class WebEnginePage: public QWebEnginePage
{
    Q_OBJECT
  public:
    using QWebEnginePage::QWebEnginePage;

  protected:
    bool acceptNavigationRequest(QUrl const &url, QWebEnginePage::NavigationType type, bool isMainFrame) override;
};

#endif // EXT_WEBENGINE_P_H
