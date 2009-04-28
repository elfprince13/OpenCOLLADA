/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of COLLADASaxFrameworkLoader.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADA_LOADER_H__
#define __COLLADA_LOADER_H__

#include "COLLADASaxFWLPrerequisites.h"
#include "COLLADASaxFWLGeometryMaterialIdInfo.h"
#include "COLLADASaxFWLSidTreeNode.h"

#include "COLLADAFWILoader.h"
#include "COLLADAFWLoaderUtils.h"
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWTypes.h"

#include "COLLADABUHashMap.h"
#include "COLLADABUHashFunctions.h"
#include "COLLADABUURI.h"


namespace COLLADAFW
{
	class IWriter;
	class VisualScene;
	class Effect;
	class AnimationList;
}


namespace COLLADASaxFWL
{

	class IErrorHandler;


	/** Loader to a COLLADA document and all the documents that are referenced it.*/
	class Loader : public COLLADAFW::ILoader 
	{
	public:
		/** Maps the unique id of each geometry to the corresponding ColladaSymbolMaterialIdMap.*/
		typedef std::map<COLLADAFW::UniqueId, GeometryMaterialIdInfo> UniqueIdMeshMaterialIdInfoMap;

		typedef std::map<String, COLLADAFW::TextureMapId> StringTextureMapIdMap;

		/** Flags for each type of object that are passed by the IWriter interface. */
		enum ObjectFlags
		{
			NO_FLAG                    = 0,
			ASSET_FLAG                 = 1<<0,
			VISUAL_SCENES_FLAG         = 1<<1,
			LIBRARY_NODES_FLAG         = 1<<2,
			GEOMETRY_FLAG              = 1<<3,
			MATERIAL_FLAG              = 1<<4,
			EFFECT_FLAG                = 1<<5,
			CAMERA_FLAG                = 1<<6,
			IMAGE_FLAG                 = 1<<7,
			LIGHT_FLAG                 = 1<<8,
			ANIMATION_FLAG             = 1<<9,
			ANIMATION_LIST_FLAG        = 1<<10,

			ALL_OBJECTS_MASK           = (1<<11) - 1,
		};

	private:
		typedef COLLADABU::HashMap<COLLADABU::URI, COLLADAFW::UniqueId, unsigned long, COLLADABU::calculateHash> URIUniqueIdMap;

		/** Maps the id of a collada element to the corresponding sit tree node.*/
		typedef std::map<String /*id*/, SidTreeNode*> IdStringSidTreeNodeMap;

		/** Maps unique ids of animation list to the corresponding animation list.*/
		typedef std::map< COLLADAFW::UniqueId , COLLADAFW::AnimationList* > UniqueIdAnimationListMap;

		/** List of visual scenes.*/
		typedef std::vector<COLLADAFW::VisualScene*> VisualSceneList;

		/** List of effects.*/
		typedef std::vector<COLLADAFW::Effect*> EffectList;


	private:
		/** Loader utils that will help us to fill the model.*/
		COLLADAFW::LoaderUtils mLoaderUtil;

		/** The writer that will be fed by this loader.*/
		COLLADAFW::IWriter* mWriter;

		/** Maps each already processed dae element to its COLLADAFW::UniqueId. */
		URIUniqueIdMap mURIUniqueIdMap;

		/** Maps the unique id of each geometry to the corresponding GeometryMaterialIdInfo.*/
		UniqueIdMeshMaterialIdInfoMap mGeometryMeshMaterialIdInfoMapMap;

		/** The error handler to pass the errors to.*/
		IErrorHandler* mErrorHandler;

		/** The TextureMapId that will be assigned to the next unknown texture map semantic.*/
		COLLADAFW::TextureMapId mNextTextureMapId;

		/** Maps the semantic name of a texture map to the TextureMapId used in the framework.*/
		StringTextureMapIdMap mTextureMapSemanticTextureMapIdMap;

		/** A combination of ObjectFlags, indicating which objects should be parsed during the 
		parse process.*/
		int mObjectFlags;

		/** A combination of ObjectFlags, indicating which objects have be parsed already.*/
		int mParsedObjectFlags;

		/** The root node of the sid tree. This tree is used to resolve sids.*/
		SidTreeNode *mSidTreeRoot;

		/** Maps the id of a collada element to the corresponding sit tree node.*/
		IdStringSidTreeNodeMap mIdStringSidTreeNodeMap;

		/** List of all visual scenes in the file. They are send to the writer and deleted, when the file has 
		completely been parsed.*/
		VisualSceneList mVisualScenes;

		/** List of all effects in the file. They are send to the writer and deleted, when the file has 
		completely been parsed.*/
		EffectList mEffects;

		/** Maps unique ids of animation list to the corresponding animation list. All animation list in this map 
		will be deleted by the FileLoader.*/
		UniqueIdAnimationListMap mUniqueIdAnimationListMap;

	public:

        /** Constructor. */
		Loader( IErrorHandler* errorHandler = 0 );

        /** Destructor. */
		virtual ~Loader();


        /** Starts loading the model and feeds the writer with data.
		@param fileName The name of the fills that should be loaded.
		@param writer The writer that should be fed with data.
		@param objectFlags The flags indicating which objects should be loaded
		@return True, if loading succeeded, false otherwise.*/
		virtual bool loadDocument(const String& fileName, COLLADAFW::IWriter* writer );

		/** Sets the flags indicating which objects should be loaded.
		@param objectFlags The flags indicating which objects should be loaded.*/
		void setObjectFlags( int objectFlags ) { mObjectFlags = objectFlags; }

	private:
		friend class IFilePartLoader;
		friend class FileLoader;

		/** Returns the COLLADAFW::UniqueId of the element with uri @a uri. If the uri has been 
		passed to this method before, the same 	COLLADAFW::UniqueId will be returned, if not, a 
		new one is created.
		@param uri The uri of the element to get the COLLADAFW::UniqueId for
		@param classId The COLLADAFW::ClassId of the object that will be created for @a element.
		@return The elements COLLADAFW::UniqueId */
		const COLLADAFW::UniqueId& getUniqueId(const COLLADABU::URI& uri, COLLADAFW::ClassId classId);

		/** Returns the COLLADAFW::UniqueId of an element with no uri.  At each call a new 
		COLLADAFW::UniqueId will be created and returned. Use this member for collada elements that
		do not have an id.
		@param classId The COLLADAFW::ClassId of the object that will be created for @a element.
		@return The elements COLLADAFW::UniqueId */
		COLLADAFW::UniqueId getUniqueId(COLLADAFW::ClassId classId);

		/** Returns the GeometryMaterialIdInfo object of the geometry with @a uniqueId. If this method has 
		not been called before with the same uniqueId, an empty GeometryMaterialIdInfo is created, added to
		the map and returned.*/
		GeometryMaterialIdInfo& getMeshMaterialIdInfo( const COLLADAFW::UniqueId& uniqueId);

		/** Returns TextureMapId for @a semantic. Successive call with same semantic return the same TextureMapId.*/
		COLLADAFW::TextureMapId getTextureMapIdBySematic( const String& semantic );

		/** The root node of the sid tree. This tree is used to resolve sids.*/
		SidTreeNode * getSidTreeRoot() { return mSidTreeRoot; }

		/** Maps the id of a collada element to the corresponding sit tree node.*/
		IdStringSidTreeNodeMap& getIdStringSidTreeNodeMap() { return mIdStringSidTreeNodeMap; }

		/** List of all visual scenes in the file. They are send to the writer and deleted, when the file has 
		completely been parsed.*/
		VisualSceneList& getVisualScenes() { return mVisualScenes; }

		/** List of all effects in the file. They are send to the writer and deleted, when the file has 
		completely been parsed.*/
		EffectList& getEffects() { return mEffects; }

		/** Maps unique ids of animation list to the corresponding animation list. All animation list in this map 
		will be deleted by the FileLoader.*/
		UniqueIdAnimationListMap& getUniqueIdAnimationListMap() { return mUniqueIdAnimationListMap; }




		/** Returns the writer the data will be written to.*/
		COLLADAFW::IWriter* writer(){ return mWriter; }


        /** Disable default copy ctor. */
		Loader( const Loader& pre );

        /** Disable default assignment operator. */
		const Loader& operator= ( const Loader& pre );


	};

} // namespace COLLADA

#endif // __COLLADA_LOADER_H__
