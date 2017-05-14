/*

Author  :   astips

Github  :   https://github.com/astips

*/


#include "mirrorPlane.h"


static float plane[][3] = { { 0.00f, 1.00f, 1.00f }, 
                            { 0.00f, -1.00f, 1.00f }, 
                            { 0.00f, -1.00f, -1.00f }, 
                            { 0.00f, 1.00f, -1.00f } };


MTypeId MirrorPlane::id( 0x71000 );
MString MirrorPlane::drawDbClassification( "drawdb/geometry/mirrorPlane_GeometryOverride" );
MString MirrorPlane::drawRegistrantId( "MirrorPlaneNode_GeometryOverridePlugin" );
MObject MirrorPlane::hyaline;
MObject MirrorPlane::size;
MObject MirrorPlane::top;
MObject MirrorPlane::bottom;
MObject MirrorPlane::front;
MObject MirrorPlane::back;
MObject MirrorPlane::asType;


void* MirrorPlane::creator()
{
    return new MirrorPlane();
}


MStatus MirrorPlane::compute( const MPlug& , MDataBlock& )
{
    return MS::kUnknownParameter;
}


bool MirrorPlane::excludeAsLocator() const
{
    MObject thisNode = thisMObject();
    MPlug asTypePlug( thisNode, MirrorPlane::asType );
    bool hideIt;
    asTypePlug.getValue( hideIt );
    return !hideIt;
}


float MirrorPlane::unitValue( const MPlug& plug )
{
    MDistance dist;
    plug.getValue( dist );
    return (float)dist.asCentimeters();
}


void MirrorPlane::draw( M3dView& view, 
                        const MDagPath& path,
                        M3dView::DisplayStyle style,
                        M3dView::DisplayStatus status )
{

    MObject thisNode = thisMObject();

    MPlug localPositionXPlug( thisNode, MirrorPlane::localPositionX );
    MPlug localPositionYPlug( thisNode, MirrorPlane::localPositionY );
    MPlug localPositionZPlug( thisNode, MirrorPlane::localPositionZ );
    float lpx = localPositionXPlug.asFloat();
    float lpy = localPositionYPlug.asFloat();
    float lpz = localPositionZPlug.asFloat();

    MPlug localScaleYPlug( thisNode, MirrorPlane::localScaleY );
    MPlug localScaleZPlug( thisNode, MirrorPlane::localScaleZ );
    float lsy = localScaleYPlug.asFloat();
    float lsz = localScaleZPlug.asFloat();

    MPlug hyalinePlug( thisNode, MirrorPlane::hyaline );
    float newHyaline = hyalinePlug.asFloat();

    MPlug sizePlug( thisNode, MirrorPlane::size );
    float newSize = unitValue( sizePlug );

    MPlug topPlug( thisNode, MirrorPlane::top );
    float newTop = unitValue( topPlug );

    MPlug bottomPlug( thisNode, MirrorPlane::bottom );
    float newBottom = unitValue( bottomPlug );

    MPlug frontPlug( thisNode, MirrorPlane::front );
    float newFront = unitValue( frontPlug );

    MPlug backPlug( thisNode, MirrorPlane::back );
    float newBack = unitValue( backPlug );

    view.beginGL();
    
    glPushAttrib( GL_CURRENT_BIT );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if ( status == M3dView::kDormant )
    {
        glColor4f( 0.5f, 0.2f, 0.1f, 1.0f * newHyaline );
    }

    if ( status == M3dView::kActive )
    {
        glColor4f( 0.5f, 1.0f, 0.5f, 0.8f * newHyaline );
    }

    if ( status == M3dView::kLead )
    {
        glColor4f( 0.75f, 1.0f, 0.75f, 1.0f * newHyaline );
    }

    if ( status == M3dView::kNoStatus )
    {
        glColor4f( 0.0f, 0.0f, 0.0f, 1.0f * newHyaline );
    }

    glBegin( GL_POLYGON );
    glVertex3f( plane[0][0] + lpx, plane[0][1] * newSize * newTop    * lsy + lpy, plane[0][2] * newSize * newFront * lsz + lpz );
    glVertex3f( plane[1][0] + lpx, plane[1][1] * newSize * newBottom * lsy + lpy, plane[1][2] * newSize * newFront * lsz + lpz );
    glVertex3f( plane[2][0] + lpx, plane[2][1] * newSize * newBottom * lsy + lpy, plane[2][2] * newSize * newBack  * lsz + lpz );
    glVertex3f( plane[3][0] + lpx, plane[3][1] * newSize * newTop    * lsy + lpy, plane[3][2] * newSize * newBack  * lsz + lpz );
    glEnd();

    glDisable( GL_BLEND );
    glPopAttrib();

    view.endGL();
}


MBoundingBox MirrorPlane::boundingBox() const
{
    MObject thisNode = thisMObject();
    MPlug plug( thisNode, size );
    MDistance sizeVal;
    plug.getValue( sizeVal );
    double multiplier = sizeVal.asCentimeters();
    MPoint corner1( 0.0,  1.0,  1.0 );
    MPoint corner2( 0.0, -1.0, -1.0 );
    corner1 = corner1 * multiplier;
    corner2 = corner2 * multiplier;
    return MBoundingBox( corner1, corner2 );
}


MStatus MirrorPlane::initialize()
{
    MStatus status;

    MFnNumericAttribute numericFn;
	MFnUnitAttribute unitFn;
    MFnEnumAttribute enumFn;

    MirrorPlane::hyaline = numericFn.create( "hyaline", "hy", MFnNumericData::kFloat, 0.5 );
    numericFn.setHidden( false );
    numericFn.setKeyable( false );
    numericFn.setChannelBox( true );
    numericFn.setMin( 0.0 );
    numericFn.setMax( 1.0 );
    addAttribute( MirrorPlane::hyaline );

    MirrorPlane::size = unitFn.create( "size", "sz", MFnUnitAttribute::kDistance, 1.0 );
    unitFn.setHidden( false );
    unitFn.setKeyable( false );
    unitFn.setChannelBox( true );
    addAttribute( MirrorPlane::size );

    MirrorPlane::top = unitFn.create( "top", "tp", MFnUnitAttribute::kDistance, 1.0 );
    unitFn.setHidden( false );
    unitFn.setKeyable( false );
    unitFn.setChannelBox( true );
    addAttribute( MirrorPlane::top );

    MirrorPlane::bottom = unitFn.create( "bottom", "bt", MFnUnitAttribute::kDistance, 1.0 );
    unitFn.setHidden( false );
    unitFn.setKeyable( false );
    unitFn.setChannelBox( true );
    addAttribute( MirrorPlane::bottom );

    MirrorPlane::front = unitFn.create( "front", "ft", MFnUnitAttribute::kDistance, 1.0 );
    unitFn.setHidden( false );
    unitFn.setKeyable( false );
    unitFn.setChannelBox( true );
    addAttribute( MirrorPlane::front );

    MirrorPlane::back = unitFn.create( "back", "bk", MFnUnitAttribute::kDistance, 1.0 );
    unitFn.setHidden( false );
    unitFn.setKeyable( false );
    unitFn.setChannelBox( true );
    addAttribute( MirrorPlane::back );

    MirrorPlane::asType = enumFn.create( "asType", "at", 0 );
    enumFn.addField( "locator", 0 );
    enumFn.addField( "custom", 1 );
    enumFn.setHidden( false );
    enumFn.setKeyable( false );
    enumFn.setChannelBox( true );
    enumFn.setStorable( true );
    addAttribute( MirrorPlane::asType );

    return MS::kSuccess;
}




/*

Viewport 2.0 Override

*/
MirrorPlaneOverride::MirrorPlaneOverride( const MObject& obj )
: MHWRender::MPxGeometryOverride( obj )
, mSolidUIShader( NULL )
, mLocatorNode( obj )
, newHyaline( 0.5f )
, newSize( 1.0f )
, newTop( 1.0f )
, newBottom( 1.0f )
, newFront( 1.0f )
, newBack( 1.0f )
, needRefresh( true )
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if ( !renderer )
        return;

    const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    if ( !shaderMgr )
        return;

    mSolidUIShader = shaderMgr->getStockShader( MHWRender::MShaderManager::k3dSolidShader );
}


MirrorPlaneOverride::~MirrorPlaneOverride()
{
    if( mSolidUIShader )
    {
        MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
        if ( renderer )
        {
            const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
            if (shaderMgr)
            {
                shaderMgr->releaseShader( mSolidUIShader );
            }
        }
        mSolidUIShader = NULL;
    }
}


MHWRender::DrawAPI MirrorPlaneOverride::supportedDrawAPIs() const
{
    return ( MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile );
}


const MString colorParameterName_        = "solidColor";
const MString wireframePlaneItemName_    = "mirrorPlaneWires";
const MString shadedPlaneItemName_       = "mirrorPlaneTriangles";


void MirrorPlaneOverride::updateDG()
{

    MPlug hyalinePlug( mLocatorNode, MirrorPlane::hyaline );
    newHyaline = (float)hyalinePlug.asFloat();

    MPlug sizePlug( mLocatorNode, MirrorPlane::size );
    MDistance sizeVal;
    sizePlug.getValue( sizeVal );
    newSize = (float)sizeVal.asCentimeters();

    MPlug topPlug( mLocatorNode, MirrorPlane::top );
    MDistance topVal;
    topPlug.getValue( topVal );
    newTop = (float)topVal.asCentimeters();

    MPlug bottomPlug( mLocatorNode, MirrorPlane::bottom );
    MDistance bottomVal;
    bottomPlug.getValue( bottomVal );
    newBottom = (float)bottomVal.asCentimeters();

    MPlug frontPlug( mLocatorNode, MirrorPlane::front );
    MDistance frontVal;
    frontPlug.getValue( frontVal );
    newFront = (float)frontVal.asCentimeters();

    MPlug backPlug( mLocatorNode, MirrorPlane::back );
    MDistance backVal;
    backPlug.getValue( backVal );
    newBack = (float)backVal.asCentimeters();

    needRefresh = true;
}


void MirrorPlaneOverride::updateRenderItems( const MDagPath& path, MHWRender::MRenderItemList& list )
{
    // MHWRender::MRenderItem* wireframePlane = NULL;
    // int index = list.indexOf( wireframePlaneItemName_ );

    // if ( index < 0 )
    // {
    //     wireframePlane = MHWRender::MRenderItem::Create( wireframePlaneItemName_,
    //                                                      MHWRender::MRenderItem::DecorationItem,
    //                                                      MHWRender::MGeometry::kLineStrip );
    //     wireframePlane->setDrawMode( MHWRender::MGeometry::kWireframe );
    //     wireframePlane->depthPriority( 5 );
    //     list.append( wireframePlane );
    // }
    // else
    // {
    //     wireframePlane = list.itemAt( index );
    // }
    // if(wireframePlane)
    // {
    //     if ( mSolidUIShader )
    //     {
    //         MColor color = MHWRender::MGeometryUtilities::wireframeColor( path );
    //         float  wireframeColor[4] = { color.r, color.g, color.b, 1.0f };
    //         mSolidUIShader->setParameter( colorParameterName_, wireframeColor );
    //         wireframePlane->setShader( mSolidUIShader );
    //     }
    //     wireframePlane->enable( false );
    // }

    MHWRender::MRenderItem* shadedPlaneItem = NULL;
    int index = list.indexOf( shadedPlaneItemName_ );
    if ( index < 0 )
    {
        shadedPlaneItem = MHWRender::MRenderItem::Create( shadedPlaneItemName_,
                                                          MHWRender::MRenderItem::DecorationItem,
                                                          MHWRender::MGeometry::kTriangleStrip );
        shadedPlaneItem->setDrawMode( MHWRender::MGeometry::kAll );
        shadedPlaneItem->depthPriority( 5 );
        list.append( shadedPlaneItem );
    }
    else
    {
        shadedPlaneItem = list.itemAt( index );
    }

    if( shadedPlaneItem )
    {
        if ( mSolidUIShader )
        {
            MColor color = MHWRender::MGeometryUtilities::wireframeColor( path );
            float wireframeColor[4] = { color.r, color.g, color.b, 1.0f * newHyaline };
            mSolidUIShader->setParameter( colorParameterName_, wireframeColor );
            shadedPlaneItem->setShader( mSolidUIShader );
        }
        shadedPlaneItem->enable( true );
    }
}


void MirrorPlaneOverride::populateGeometry( const MHWRender::MGeometryRequirements &requirements, 
                                            const MHWRender::MRenderItemList &renderItems, 
                                            MHWRender::MGeometry &data )
{
    MHWRender::MVertexBuffer* verticesBuffer = NULL;
    float* vertices = NULL;
    const MHWRender::MVertexBufferDescriptorList& vertexBufferDescriptorList = requirements.vertexRequirements();
    const int numberOfVertexRequirments = vertexBufferDescriptorList.length();
    MHWRender::MVertexBufferDescriptor vertexBufferDescriptor;

    for ( int requirmentNumber=0; requirmentNumber<numberOfVertexRequirments; ++requirmentNumber )
    {
        if ( !vertexBufferDescriptorList.getDescriptor( requirmentNumber, vertexBufferDescriptor ) )
        {
            continue;
        }

        switch ( vertexBufferDescriptor.semantic() )
        {
        case MHWRender::MGeometry::kPosition:
            {
                if ( !verticesBuffer )
                {
                    verticesBuffer = data.createVertexBuffer( vertexBufferDescriptor );
                    if ( verticesBuffer )
                    {
                        vertices = (float*)verticesBuffer->acquire( 4 );
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    MPlug localPositionXPlug( mLocatorNode, MirrorPlane::localPositionX );
    MPlug localPositionYPlug( mLocatorNode, MirrorPlane::localPositionY );
    MPlug localPositionZPlug( mLocatorNode, MirrorPlane::localPositionZ );
    float lpx = localPositionXPlug.asFloat();
    float lpy = localPositionYPlug.asFloat();
    float lpz = localPositionZPlug.asFloat();

    MPlug localScaleYPlug( mLocatorNode, MirrorPlane::localScaleY );
    MPlug localScaleZPlug( mLocatorNode, MirrorPlane::localScaleZ );
    float lsy = localScaleYPlug.asFloat();
    float lsz = localScaleZPlug.asFloat();

    vertices[0]  = plane[0][0] + lpx;
    vertices[1]  = plane[0][1] * newSize * newTop    * lsy + lpy;
    vertices[2]  = plane[0][2] * newSize * newFront  * lsz + lpz;

    vertices[3]  = plane[1][0] + lpx;
    vertices[4]  = plane[1][1] * newSize * newBottom * lsy + lpy;
    vertices[5]  = plane[1][2] * newSize * newFront  * lsz + lpz;

    vertices[6]  = plane[2][0] + lpx;
    vertices[7]  = plane[2][1] * newSize * newBottom * lsy + lpy;
    vertices[8]  = plane[2][2] * newSize * newBack   * lsz + lpz;

    vertices[9]  = plane[3][0] + lpx;
    vertices[10] = plane[3][1] * newSize * newTop    * lsy + lpy;
    vertices[11] = plane[3][2] * newSize * newBack   * lsz + lpz;

    if( verticesBuffer && vertices )
    {
        verticesBuffer->commit( vertices );
    }

    for ( int i=0; i<renderItems.length(); ++i )
    {
        const MHWRender::MRenderItem* item = renderItems.itemAt( i );
        if ( !item )
        {
            continue;
        }
        int startIndex = 0;
        int endIndex = 3;
        int numIndex = 4;
        bool isWireFrame = false;

        if ( numIndex )
        {
            MHWRender::MIndexBuffer* indexBuffer = data.createIndexBuffer( MHWRender::MGeometry::kUnsignedInt32 );
            unsigned int* indices = (unsigned int*)indexBuffer->acquire( numIndex );
            for( int i = 0; i < numIndex; )
            {
                if ( isWireFrame )
                {
                    indices[i] = startIndex + i;
                    i++;
                }
                else
                {
                    indices[i] = startIndex + i/2;
                    if ( i+1 < numIndex )
                        indices[i+1] = endIndex - i/2;
                    i += 2;
                }
            }
            indexBuffer->commit( indices );
            item->associateWithIndexBuffer( indexBuffer );
        }
    }
    needRefresh = false;
}


MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, AUTHOR_NAME, "3.0", "Any");
    status = plugin.registerNode( PLUGIN_NODE_NAME,
                                  MirrorPlane::id,
                                  &MirrorPlane::creator,
                                  &MirrorPlane::initialize,
                                  MPxNode::kLocatorNode,
                                  &MirrorPlane::drawDbClassification );
    if ( !status )
    {
        status.perror( "registerNode" );
        return status;
    }


    status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator( MirrorPlane::drawDbClassification,
                                                                        MirrorPlane::drawRegistrantId, 
                                                                        MirrorPlaneOverride::Creator );
    if ( !status )
    {
        status.perror( "registerDrawOverrideCreator" );
        return status;
    }

    return status;
}


MStatus uninitializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator( MirrorPlane::drawDbClassification,
                                                                          MirrorPlane::drawRegistrantId );

    if ( !status )
    {
        status.perror( "deregisterDrawOverrideCreator" );
        return status;
    }

    status = plugin.deregisterNode( MirrorPlane::id );
    if ( !status )
    {
        status.perror( "deregisterNode" );
        return status;
    }
    return status;
}
