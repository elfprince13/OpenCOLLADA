/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of COLLADASaxFrameworkLoader.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADASaxFWLStableHeaders.h"
#include "COLLADASaxFWLAssetLoader.h"
#include "COLLADASaxFWLLoader.h"


namespace COLLADASaxFWL
{

    //------------------------------
    AssetLoader::AssetLoader ( IFilePartLoader* callingFilePartLoader ) 
        : FilePartLoader ( callingFilePartLoader )
        , mAsset ( new COLLADAFW::FileInfo () )
	{
		mAsset->setAbsoluteFileUri( callingFilePartLoader->getFileUri() );
		// COLLADA always uses degrees as angular units
		mAsset->getUnit().setAngularUnit( COLLADAFW::FileInfo::Unit::DEGREES );
	}
	
    //------------------------------
	AssetLoader::~AssetLoader()
	{
	}

	//------------------------------
	bool AssetLoader::end__asset()
	{
		SaxVirtualFunctionTest(end__asset());
		bool success = true;
		if ( (getObjectFlags() & Loader::ANIMATION_FLAG) != 0 )
		{
			success = writer()->writeGlobalAsset ( mAsset );
		}
		delete mAsset;
		finish();
		return success;
	}

	//------------------------------
	bool AssetLoader::data__author( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__author(data, length));
		mAsset->appendValuePair ( "author", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__authoring_tool( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__authoring_tool(data, length));
		mAsset->appendValuePair ( "authoring_tool", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__copyright( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__copyright(data, length)); 
		mAsset->appendValuePair ( "copyright", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__comments( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__comments(data, length)); 
		mAsset->appendValuePair ( "comments", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__source_data( COLLADABU::URI value )
	{
		SaxVirtualFunctionTest(data__source_data(value)); 
		mAsset->appendValuePair ( "source", value.getURIString() );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__created( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__created(data, length)); 
		mAsset->appendValuePair ( "created", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__keywords( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__keywords(data, length)); 
		mAsset->appendValuePair ( "keywords", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__modified( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__modified(data, length)); 
		mAsset->appendValuePair ( "modified", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__revision( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__revision(data, length)); 
		mAsset->appendValuePair ( "revision", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__subject( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__subject(data, length)); 
		mAsset->appendValuePair ( "subject", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__title( const ParserChar* data, size_t length )
	{
		SaxVirtualFunctionTest(data__title(data, length)); 
		mAsset->appendValuePair ( "title", String ( (char*) data, length ) );
		return true;
	}

	//------------------------------
	bool AssetLoader::begin__unit( const unit__AttributeData& attributeData )
	{
		SaxVirtualFunctionTest(begin__unit(attributeData)); 
		mAsset->getUnit().setLinearUnitName ( String ( (char*) attributeData.name ) );
		mAsset->getUnit().setLinearUnitMeter ( attributeData.meter );
		return true;
	}

	//------------------------------
	bool AssetLoader::data__up_axis( const COLLADASaxFWL::ENUM__UpAxisType val )
	{
		SaxVirtualFunctionTest(data__up_axis(val)); 
		switch ( val )
		{
		case COLLADASaxFWL::ENUM__UpAxisType__X_UP:
			mAsset->setUpAxisType ( COLLADAFW::FileInfo::X_UP );
			break;
		case COLLADASaxFWL::ENUM__UpAxisType__Y_UP:
			mAsset->setUpAxisType ( COLLADAFW::FileInfo::Y_UP );
			break;
		case COLLADASaxFWL::ENUM__UpAxisType__Z_UP:
			mAsset->setUpAxisType ( COLLADAFW::FileInfo::Z_UP );
			break;
		default:
			mAsset->setUpAxisType ( COLLADAFW::FileInfo::Y_UP);
		}
		return true;
	}
} // namespace COLLADASaxFWL
