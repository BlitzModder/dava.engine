#include "GradientSlider.h"

#include <QPainter>

#include "../Helpers/PaintingHelper.h"


GradientSlider::GradientSlider(QWidget *parent)
    : AbstractSlider(parent)
    , arrowSize( 9, 9 )
    , orientation( Qt::Horizontal )
    , bgBrush( PaintingHelper::BuildGridBrush( QSize( 5, 5 ) ) )
{
}

GradientSlider::~GradientSlider()
{
}

void GradientSlider::SetColors( const QColor& _c1, const QColor& _c2 )
{
    c1 = _c1;
    c2 = _c2;
    update();
}

void GradientSlider::SetDimensions( const Qt::Edges& flags )
{
    arrows = flags;
    update();
}

void GradientSlider::SetOrientation( Qt::Orientation _orientation )
{
    orientation = _orientation;
    update();
}

void GradientSlider::DrawBackground( QPainter* p ) const
{
    const QRect& rc = PosArea();

    if ( bgCache .isNull() )
    {
        const QImage& bg = PaintingHelper::BuildGradient( rc.size(), c1, c2, orientation );
        bgCache = QPixmap::fromImage( bg );
    }

    p->fillRect( rc, bgBrush );
    p->drawPixmap( rc.topLeft(), bgCache );
}

void GradientSlider::DrawForeground( QPainter* p ) const
{
    Qt::Edges flags;
    switch ( orientation )
    {
    case Qt::Horizontal:
        flags = arrows & ( Qt::TopEdge | Qt::BottomEdge );
        break;
    case Qt::Vertical:
        flags = arrows & ( Qt::LeftEdge | Qt::RightEdge );
        break;
    }

    if ( flags.testFlag( Qt::TopEdge ) )
        drawArrow( Qt::TopEdge, p );
    if ( flags.testFlag( Qt::LeftEdge ) )
        drawArrow( Qt::LeftEdge, p );
    if ( flags.testFlag( Qt::RightEdge ) )
        drawArrow( Qt::RightEdge, p );
    if ( flags.testFlag( Qt::BottomEdge ) )
        drawArrow( Qt::BottomEdge, p );
}

void GradientSlider::resizeEvent( QResizeEvent* e )
{
    bgCache = QPixmap();
    AbstractSlider::resizeEvent( e );
}

void GradientSlider::drawArrow( Qt::Edge arrow, QPainter *p ) const
{
    const auto it = arrowCache.constFind( arrow );
    if ( it == arrowCache.constEnd() )
    {
        arrowCache[arrow] = QPixmap::fromImage( PaintingHelper::BuildArrowIcon( arrowSize, arrow, Qt::black ) );
    }

    const QPoint& currentPos = Pos();
    QPoint pos;

    switch ( arrow )
    {
    case Qt::TopEdge:
        pos.setX( currentPos.x() - arrowSize.width() / 2 );
        pos.setY( 0 );
        break;
    case Qt::LeftEdge:
        pos.setX( 0 );
        pos.setY( currentPos.y() - arrowSize.height() / 2 );
        break;
    case Qt::RightEdge:
        pos.setX( width() - arrowSize.width() + 1 );
        pos.setY( currentPos.y() - arrowSize.height() / 2 );
        break;
    case Qt::BottomEdge:
        pos.setX( currentPos.x() - arrowSize.width() / 2 );
        pos.setY( height() - arrowSize.height() + 1 );
        break;
    default:
        return;
    }

    QRect rc( pos, arrowSize );
    p->drawPixmap( pos, arrowCache[arrow] );
}
