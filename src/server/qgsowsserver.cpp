/***************************************************************************
                              qgsowsserver.cpp
                              -------------------
  begin                : February 27, 2012
  copyright            : (C) 2012 by René-Luc D'Hont & Marco Hugentobler
  email                : rldhont at 3liz dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsowsserver.h"
#include "qgsmaplayerregistry.h"
#include "qgsmessagelog.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"


#ifdef HAVE_SERVER_PYTHON_PLUGINS
/** Apply filter from AccessControl */
void QgsOWSServer::applyAccessControlLayerFilters( QgsMapLayer* mapLayer, QMap<QString, QString>& originalLayerFilters ) const
{
  if ( QgsVectorLayer* layer = dynamic_cast<QgsVectorLayer*>( mapLayer ) )
  {
    if ( layer->setSubsetString( "" ) )
    {
      QString sql = mAccessControl->extraSubsetString( layer );
      if ( !sql.isEmpty() )
      {
        if ( !originalLayerFilters.contains( layer->id() ) )
        {
          originalLayerFilters.insert( layer->id(), layer->subsetString() );
        }
        if ( !layer->subsetString().isEmpty() )
        {
          sql.prepend( " AND " );
          sql.prepend( layer->subsetString() );
        }
        layer->setSubsetString( sql );
      }
    }
    else
    {
      QgsMessageLog::logMessage( "Layer does not support Subset String" );
    }
  }
}
#endif

/** Restore layer filter as original */
void QgsOWSServer::restoreLayerFilters( const QMap<QString, QString>& filterMap ) const
{
  QMap<QString, QString>::const_iterator filterIt = filterMap.constBegin();
  for ( ; filterIt != filterMap.constEnd(); ++filterIt )
  {
    QgsVectorLayer* filteredLayer = dynamic_cast<QgsVectorLayer*>( QgsMapLayerRegistry::instance()->mapLayer( filterIt.key() ) );
    if ( filteredLayer )
    {
      QgsVectorDataProvider* dp = filteredLayer->dataProvider();
      if ( dp )
      {
        dp->setSubsetString( filterIt.value() );
      }
    }
  }
}
