/*

Author  :   astips

Github  :   https://github.com/astips

*/


#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MColor.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnEnumAttribute.h>
// Viewport 2.0 includes
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/M3dView.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MDrawRegistry.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>


#define AUTHOR_NAME         "astips"
#define PLUGIN_NODE_NAME    "mirrorPlane"


class MirrorPlane : public MPxLocatorNode
{
public:
                            MirrorPlane() {};
    virtual                 ~MirrorPlane() {};

    virtual bool            isBounded() const { return true; }
    virtual bool            isTransparent() const { return true; }
    virtual bool            excludeAsLocator() const;
    virtual MBoundingBox    boundingBox() const;

    virtual void            draw( M3dView & view, 
                                  const MDagPath & path,
                                  M3dView::DisplayStyle style,
                                  M3dView::DisplayStatus status );

    static  void *          creator();
    static  MStatus         initialize();
    virtual MStatus         compute( const MPlug& plug, MDataBlock& data );

public:
    static  MTypeId         id;

    static  MObject         hyaline; 
    static  MObject         size; 
    static  MObject         top; 
    static  MObject         bottom; 
    static  MObject         front; 
    static  MObject         back; 
    static  MObject         asType;

    static  MString         drawDbClassification;
    static  MString         drawRegistrantId;

private:
    float                   unitValue( const MPlug& plug );
};




class MirrorPlaneOverride : public MHWRender::MPxGeometryOverride
{
public:
    static MHWRender::MPxGeometryOverride* Creator( const MObject& obj )
    {
        return new MirrorPlaneOverride( obj );
    }

    virtual                 ~MirrorPlaneOverride();
    virtual                 MHWRender::DrawAPI supportedDrawAPIs() const;
    virtual bool            hasUIDrawables() const { return false; }
    virtual void            updateDG();

    virtual bool            isStreamDirty( const MHWRender::MVertexBufferDescriptor &desc ) { return needRefresh; }
    virtual bool            isIndexingDirty( const MHWRender::MRenderItem &item ) { return false; }

    virtual void            updateRenderItems( const MDagPath &path, MHWRender::MRenderItemList& list );
    
    virtual void            populateGeometry( const MHWRender::MGeometryRequirements &requirements, 
                                              const MHWRender::MRenderItemList &renderItems, 
                                              MHWRender::MGeometry &data );
    virtual void            cleanUp() {};

    virtual bool            traceCallSequence() const
    {
        return false;
    }

    virtual void            handleTraceMessage( const MString &message ) const
    {
        MGlobal::displayInfo( "MirrorPlaneOverride: " + message );
        fprintf( stderr, "MirrorPlaneOverride: " );
        fprintf( stderr, message.asChar() );
        fprintf( stderr, "\n" );
    }

private:
                            MirrorPlaneOverride( const MObject& obj );

    float                   newHyaline;
    float                   newSize;
    float                   newTop;
    float                   newBottom;
    float                   newFront;
    float                   newBack;

    bool                    needRefresh;

    MObject                 mLocatorNode;

    MHWRender::MShaderInstance* mSolidUIShader;
};
