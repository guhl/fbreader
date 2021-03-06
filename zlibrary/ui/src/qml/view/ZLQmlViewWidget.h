/*
 * Copyright (C) 2004-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __ZLQTVIEWWIDGET_H__
#define __ZLQTVIEWWIDGET_H__

#if QT5
#include <QQuickView>
#include <QQuickPaintedItem>
#else
#include <QDeclarativeView>
#include <QDeclarativeItem>
#endif

#include "../../../../core/src/view/ZLViewWidget.h"
#include <ZLApplication.h>

class QGridLayout;
class QScrollBar;
class ZLQmlViewWidget;
class ZLQmlBookContent;
class ZLQmlScrollBarInfo;

class ZLQmlViewObject : public QObject, public ZLViewWidget {
	Q_OBJECT
#if QT5
    Q_PROPERTY(QQuickItem* bookView READ bookView WRITE setBookView NOTIFY bookViewChanged)
#else
    Q_PROPERTY(QDeclarativeItem* bookView READ bookView WRITE setBookView NOTIFY bookViewChanged)
#endif
	Q_PROPERTY(QObject* verticalScrollBar READ verticalScrollBar CONSTANT)
	Q_PROPERTY(QObject* horizontalScrollBar READ horizontalScrollBar CONSTANT)
	
public:
	ZLQmlViewObject(ZLApplication *application);
#if QT5
    QWindow *widget();
    QQuickItem *bookView() const;
    void setBookView(QQuickItem *bookView);
#else
    QWidget *widget();
    QDeclarativeItem *bookView() const;
    void setBookView(QDeclarativeItem *bookView);
#endif
	QObject *verticalScrollBar() const;
	QObject *horizontalScrollBar() const;
	
	Q_INVOKABLE void handlePress(int x, int y);
	Q_INVOKABLE void handleRelease(int x, int y);
	Q_INVOKABLE void handleMove(int x, int y);
	Q_INVOKABLE void handleMovePressed(int x, int y);
	Q_INVOKABLE void handleFingerTap(int x, int y);
	Q_INVOKABLE void handleScrollBarMoved(int full, int from, int to);
	Q_INVOKABLE void handleScrollBarStep(int steps);
	Q_INVOKABLE void handleScrollBarPageStep(int steps);

private Q_SLOTS:
	void onVerticalSliderMoved(int value);
	void onHorizontalSliderMoved(int value);
	void onVerticalSliderClicked(int value);
	void onHorizontalSliderClicked(int value);
	
signals:
	void bookViewChanged(QObject *bookView);

private:
	void repaint();
	void trackStylus(bool track);

	void setScrollbarEnabled(ZLView::Direction direction, bool enabled);
	void setScrollbarPlacement(ZLView::Direction direction, bool standard);
	void setScrollbarParameters(ZLView::Direction direction, size_t full, size_t from, size_t to);

	QScrollBar *addScrollBar(QGridLayout *layout, Qt::Orientation orientation, int x, int y);

private:
#if QT5
    QQuickView *myWidget;
#else
    QDeclarativeView *myWidget;
#endif
//	ZLQmlViewWidget *myWidget;
	ZLQmlBookContent *myContent;
	ZLQmlScrollBarInfo *myVerticalInfo;
	ZLQmlScrollBarInfo *myHorizontalInfo;
//	int myPageSize;

//	QScrollBar *myRightScrollBar;
//	QScrollBar *myLeftScrollBar;
//	bool myShowScrollBarAtRight;

//	QScrollBar *myBottomScrollBar;
//	QScrollBar *myTopScrollBar;
//	bool myShowScrollBarAtBottom;

	ZLApplication *myApplication;
};

class ZLQmlScrollBarInfo : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
	Q_PROPERTY(int height READ height NOTIFY heightChanged)
	Q_PROPERTY(int top READ top NOTIFY topChanged)
	Q_PROPERTY(int bottom READ bottom NOTIFY bottomChanged)
public:
	ZLQmlScrollBarInfo(QObject *parent = 0);
	virtual ~ZLQmlScrollBarInfo();
	
	void setEnabled(bool enabled);
	void setParameters(int full, int from, int to);
	
	bool enabled() const;
	int height() const;
	int top() const;
	int bottom() const;
	
Q_SIGNALS:
	void enabledChanged(bool enabled) const;
	void heightChanged(int height) const;
	void topChanged(int top) const;
	void bottomChanged(int bottom) const;

private:
	bool myEnabled;
	int myHeight;
	int myTop;
	int myBottom;
};

// #if QT5
// class ZLQmlViewWidget : public QQuickView {
// #else
// class ZLQmlViewWidget : public QDeclarativeView {
// #endif
	
// public:
// #if QT5
    // ZLQmlViewWidget(QWindow *parent, ZLQmlViewObject &holder);
// #else
    // ZLQmlViewWidget(QWidget *parent, ZLQmlViewObject &holder);
// #endif

// private:
//  void keyPressEvent(QKeyEvent *event);
// //	void paintEvent(QPaintEvent *event);
// //	void mousePressEvent(QMouseEvent *event);
// //	void mouseReleaseEvent(QMouseEvent *event);
// //	void mouseMoveEvent(QMouseEvent *event);

// //	int x(const QMouseEvent *event) const;
// //	int y(const QMouseEvent *event) const;

// private:
	// ZLQmlViewObject &myHolder;
// };

#if QT5
class ZLQmlBookContent : public QQuickPaintedItem {
#else
class ZLQmlBookContent : public QDeclarativeItem {
#endif
	Q_OBJECT
	Q_PROPERTY(QObject* holder READ objectHolder WRITE setObjectHolder NOTIFY objectHolderChanged)
	Q_PROPERTY(int visibleHeight READ visibleHeight WRITE setVisibleHeight NOTIFY visibleHeightChanged)

public:
#if QT5
    ZLQmlBookContent(QQuickItem *parent = 0);
#else
    ZLQmlBookContent(QDeclarativeItem *parent = 0);
#endif
	virtual ~ZLQmlBookContent();
	
	bool eventFilter(QObject *, QEvent *);
	
	ZLQmlViewObject *objectHolder() const;
	void setObjectHolder(QObject *objectHolder);
	int visibleHeight() const;
	void setVisibleHeight(int visibleHeight);
#if QT5
    void paint(QPainter *painter);
#else
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
#endif
	bool sceneEvent(QEvent *event);
//	void mousePressEvent(QGraphicsSceneMouseEvent *event);
//	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	
public Q_SLOTS:
	void repaint();
	
Q_SIGNALS:
	void objectHolderChanged(QObject *objectHolder);
	void visibleHeightChanged(int visibleHeight);
    void tap(QObject *gesture);
	void swipe(QObject *gesture);
	
private:
	QPixmap myPixmap;
	ZLQmlViewObject *myHolder;
	int myVisibleHeight;
//	QPointF myMousePos;
};

class MyEventFilterObject : public QObject {
protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif /* __ZLQTVIEWWIDGET_H__ */
