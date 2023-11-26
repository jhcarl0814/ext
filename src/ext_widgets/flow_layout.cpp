#include <ext_widgets/flow_layout.hpp>
#include <QWidget>
FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent),
      m_hSpace(hSpacing),
      m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing),
      m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}
FlowLayout::~FlowLayout()
{
    QLayoutItem *item;
    while((item = takeAt(0)))
        delete item;
}
void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}
int FlowLayout::horizontalSpacing() const
{
    if(m_hSpace >= 0) {
        return m_hSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowLayout::verticalSpacing() const
{
    if(m_vSpace >= 0) {
        return m_vSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}
int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if(index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

QLayoutItem *FlowLayout::replaceWidget(QWidget *from, QWidget *to, Qt::FindChildOptions options)
{
    if(!from || !to)
        return nullptr;
    if(from == to) // Do not return a QLayoutItem for \a from, since ownership still
        return nullptr; // belongs to the layout (since nothing was changed)
    int index = -1;
    QLayoutItem *item = nullptr;
    for(int u = 0; u < count(); ++u) {
        item = itemAt(u);
        if(!item)
            continue;
        if(item->widget() == from) {
            index = u;
            break;
        }
        if(item->layout() && (options & Qt::FindChildOption::FindChildrenRecursively)) {
            QLayoutItem *r = item->layout()->replaceWidget(from, to, options);
            if(r)
                return r;
        }
    }
    if(index == -1)
        return nullptr;
    addChildWidget(to);
    QLayoutItem *newitem = new QWidgetItem(to);
    newitem->setAlignment(item->alignment());
    itemList.replace(index, newitem);
    return item;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return {};
}
bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}
void FlowLayout::setGeometry(QRect const &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for(QLayoutItem const *item : std::as_const(itemList))
        size = size.expandedTo(item->minimumSize());

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}
int FlowLayout::doLayout(QRect const &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    for(QLayoutItem *item : std::as_const(itemList)) {
        QWidget const *wid = item->widget();
        int spaceX = horizontalSpacing();
        if(spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Orientation::Horizontal);
        int spaceY = verticalSpacing();
        if(spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Orientation::Vertical);
        int nextX = x + item->sizeHint().width() + spaceX;
        if(nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if(!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}
int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if(!parent) {
        return -1;
    }
    else if(parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
