#include "ext_mimedata.hpp"

#include <QBuffer>

#include <QImage>

// https://stackoverflow.com/questions/13762140/proper-way-to-copy-a-qmimedata-object
// https://stackoverflow.com/questions/59809111/how-to-save-the-image-in-clipboard-in-variable-and-restore-it-to-clipboard-later
QMimeData *copyMimeData(QMimeData const *mimeReference)
{
    QMimeData *mimeCopy = new QMimeData();

    foreach(QString format, mimeReference->formats())
    {
        // Retrieving data
        QByteArray data = mimeReference->data(format);
        // Checking for custom MIME types
        //        if(format.startsWith("application/x-qt"))
        //        {
        //            // Retrieving true format name
        //            int indexBegin = format.indexOf('"') + 1;
        //            int indexEnd = format.indexOf('"', indexBegin);
        //            format = format.mid(indexBegin, indexEnd - indexBegin);
        //        }
        //        mimeCopy->setData(format, data);
        if(format == "application/x-qt-image")
        {
            mimeCopy->setImageData(mimeReference->imageData());
        }
        else
        {
            mimeCopy->setData(format, data);
        }
    }

    return mimeCopy;
}
