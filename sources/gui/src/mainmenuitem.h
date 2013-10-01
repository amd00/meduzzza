
#ifndef _MAINMENUITEM_H_
#define _MAINMENUITEM_H_

#include <QIcon>

class MainMenuItem
{
private:
	QString m_text;
	QIcon m_icon;
	QSharedPointer<QObject> m_obj;
	
public:
	MainMenuItem(const QString &_text, const QString &_icon, const QSharedPointer<QObject> &_obj) :
			m_text(_text), m_icon(_icon), m_obj(_obj) {}
	~MainMenuItem() {}
	
	QString text() const { return m_text; }
	QIcon icon() const { return m_icon; }
	QSharedPointer<QObject> object() const { return m_obj; }
};

typedef QList<MainMenuItem> MenuList;

#endif
