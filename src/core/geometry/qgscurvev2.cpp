/***************************************************************************
                         qgscurvev2.cpp
                         --------------
    begin                : November 2014
    copyright            : (C) 2014 by Marco Hugentobler
    email                : marco at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgscurvev2.h"
#include "qgslinestringv2.h"

QgsCurveV2::QgsCurveV2(): QgsAbstractGeometryV2()
{}

QgsCurveV2::~QgsCurveV2()
{}

bool QgsCurveV2::isClosed() const
{
  if ( numPoints() == 0 )
    return false;

  //don't consider M-coordinates when testing closedness
  QgsPointV2 start = startPoint();
  QgsPointV2 end = endPoint();
  return ( qgsDoubleNear( start.x(), end.x(), 1E-8 ) &&
           qgsDoubleNear( start.y(), end.y(), 1E-8 ) &&
           qgsDoubleNear( start.z(), end.z(), 1E-8 ) );
}

bool QgsCurveV2::isRing() const
{
  return ( isClosed() && numPoints() >= 4 );
}

void QgsCurveV2::coordinateSequence( QList< QList< QList< QgsPointV2 > > >& coord ) const
{
  coord.clear();
  QList<QgsPointV2> pts;
  points( pts );
  QList< QList<QgsPointV2> > ptsList;
  ptsList.append( pts );
  coord.append( ptsList );
}

bool QgsCurveV2::nextVertex( QgsVertexId& id, QgsPointV2& vertex ) const
{
  if ( id.vertex < 0 )
  {
    id.vertex = 0;
    if ( id.part < 0 )
    {
      id.part = 0;
    }
    if ( id.ring < 0 )
    {
      id.ring = 0;
    }
  }
  else
  {
    if ( id.vertex + 1 >= numPoints() )
    {
      return false;
    }
    ++id.vertex;
  }
  return pointAt( id.vertex, vertex, id.type );
}

QgsCurveV2* QgsCurveV2::segmentize() const
{
  return curveToLine();
}

QgsPointV2 QgsCurveV2::vertexAt( QgsVertexId id ) const
{
  QgsPointV2 v;
  QgsVertexId::VertexType type;
  pointAt( id.vertex, v, type );
  return v;
}
