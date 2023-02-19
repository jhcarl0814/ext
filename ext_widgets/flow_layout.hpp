#ifndef FLOW_LAYOUT_H
#define FLOW_LAYOUT_H

#include <QLayout>
#include <QStyle>

class FlowLayout: public QLayout //https://doc.qt.io/qt-6/qtwidgets-layouts-flowlayout-example.html
{
  public:
    explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~FlowLayout();

    void addItem(QLayoutItem *item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(QRect const &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;
    QLayoutItem *replaceWidget(QWidget *from, QWidget *to, Qt::FindChildOptions options) override;

  private:
    int doLayout(QRect const &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // FLOW_LAYOUT_H
