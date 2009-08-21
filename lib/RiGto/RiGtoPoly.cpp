//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//

#include <RiGto/RiGtoPoly.h>
#include <RiGto/RiGtoException.h>
#include <RiGtoStub/Stubs.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <Gto/Protocols.h>

using namespace RiGtoStub;

namespace RiGto {

//******************************************************************************
Poly::Poly( const std::string &name, 
            const unsigned int protocolVersion,
            Reader *reader )
  : Object( name, protocolVersion, reader ),
    m_numVerts( NULL ),
    m_numVertsInt( NULL ),
    m_numVertsSize( 0 ),
    m_indices( NULL ),
    m_indicesSize( 0 ),
    m_positionsRef( NULL ),
    m_positionsOpen( NULL ),
    m_positionsClose( NULL ),
    m_positionsSize( 0 ),
    m_stValues( NULL ),
    m_stValuesSize( 0 ),
    m_stIndices( NULL ),
    m_stIndicesSize( 0 ),
    m_normalIndices( NULL ),
    m_normalIndicesSize( 0 ),
    m_normalValuesRef( NULL ),
    m_normalValuesOpen( NULL ),
    m_normalValuesClose( NULL ),
    m_normalValuesSize( 0 ),
    m_smoothingMethod( 0 )
{
    // Nothing
}

//******************************************************************************
Poly::~Poly()
{
    delete[] m_numVerts;
    delete[] m_numVertsInt;
    delete[] m_indices;
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    delete[] m_stValues;
    delete[] m_stIndices;
    delete[] m_normalValuesRef;
    delete[] m_normalValuesOpen;
    delete[] m_normalValuesClose;
    delete[] m_normalIndices;
}

//******************************************************************************
//******************************************************************************
// READER STUFF
//******************************************************************************
//******************************************************************************

//******************************************************************************
void *Poly::component( const std::string &name,
                       ReaderPhase rp ) const
{
    // During the reference phase, we read:
    // points, elements, indices
    if ( rp == READER_REF )
    {
        if ( name == GTO_COMPONENT_POINTS )
        {
            return ( void * )POINTS_C;
        }
        else if ( name == GTO_COMPONENT_ELEMENTS )
        {
            return ( void * )ELEMENTS_C;
        }
        else if ( name == GTO_COMPONENT_INDICES )
        {
            return ( void * )INDICES_C;
        }
        else if ( name == GTO_COMPONENT_MAPPINGS )
        {
            return ( void * )MAPPINGS_C;
        }
        else if ( name == GTO_COMPONENT_SMOOTHING )
        {
            return ( void * )SMOOTHING_C;
        }
        else if ( name == GTO_COMPONENT_NORMALS )
        {
            return ( void * )NORMALS_C;
        }
    }
    // During the open & close phases, we read:
    // points only.
    else if ( rp == READER_OPEN ||
              rp == READER_CLOSE )
    {
        if ( name == GTO_COMPONENT_POINTS )
        {
            return ( void * )POINTS_C;
        }
        else if ( name == GTO_COMPONENT_NORMALS )
        {
            return ( void * )NORMALS_C;
        }
    }

    // Superclass
    return Object::component( name, rp );
}

//******************************************************************************
void *Poly::property( const std::string &name,
                      void *componentData,
                      ReaderPhase rp ) const
{
    // During the reference phase, we read:
    // points.positions
    // elements.primitives
    // indices.vertices
    //
    // During the open & close phase, we read:
    // points.positions

    if ( rp == READER_REF )
    {
        if ( (( int )componentData) == POINTS_C )
        {
            if ( name == GTO_PROPERTY_POSITION )
            {
                return ( void * )POINTS_POSITION_P;
            }
        }
        else if ( (( int )componentData ) == ELEMENTS_C )
        {
            if ( name == GTO_PROPERTY_SIZE )
            {
                return ( void * )ELEMENTS_SIZE_P;
            }
        }
        else if ( (( int )componentData ) == INDICES_C )
        {
            if ( name == GTO_PROPERTY_VERTEX )
            {
                return ( void * )INDICES_VERTEX_P;
            }
            else if ( name == GTO_PROPERTY_ST )
            {
                return ( void * )INDICES_ST_P;
            }
            else if ( name == GTO_PROPERTY_NORMAL )
            {
                return ( void * )INDICES_NORMAL_P;
            }
        }
        else if ( (( int )componentData ) == MAPPINGS_C )
        {
            if ( name == GTO_PROPERTY_ST )
            {
                return ( void * )MAPPINGS_ST_P;
            }
        }
        else if ( (( int )componentData ) == NORMALS_C )
        {
            if ( name == GTO_PROPERTY_NORMAL )
            {
                return ( void * )NORMALS_NORMAL_P;
            }
        }
        else if ( (( int )componentData ) == SMOOTHING_C )
        {
            if ( name == GTO_PROPERTY_METHOD )
            {
                return ( void * )SMOOTHING_METHOD_P;
            }
        }
    }
    else if ( rp == READER_OPEN ||
              rp == READER_CLOSE )
    {
        if ( (( int )componentData) == POINTS_C )
        {
            if ( name == GTO_PROPERTY_POSITION )
            {
                return ( void * )POINTS_POSITION_P;
            }
        }
        else if ( (( int )componentData ) == NORMALS_C )
        {
            if ( name == GTO_PROPERTY_NORMAL )
            {
                return ( void * )NORMALS_NORMAL_P;
            }
        }
    }

    // Superclass
    return Object::property( name, componentData, rp );
}

//******************************************************************************
#define WEIRD_SIZE( NAME, PROP )                                \
{                                                               \
    std::cerr << "ERROR: Bad size loading property: " << (PROP) \
              << " from surface: " << (NAME) << std::endl;      \
    return NULL;                                                \
}

//******************************************************************************
void *Poly::data( void *componentData,
                  void *propertyData,
                  size_t numItems,
                  size_t itemWidth,
                  size_t numBytes,
                  ReaderPhase rp )
{
    // FLOAT
    if ( rp == READER_REF )
    {
        if ( ((int)propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }
            
            return positionsRefData( numItems * 3 );
        }
        else if ( ((int)propertyData) == MAPPINGS_ST_P )
        {
            if ( itemWidth != 2 ||
                 ( numItems * 2 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "mappings.st" );
            }

            return stValuesData( numItems * 2 );
        }
        else if ( ((int)propertyData) == NORMALS_NORMAL_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "normals.normal" );
            }
            
            return normalValuesRefData( numItems * 3 );
        }
    }
    else if ( rp == READER_OPEN )
    {
        if ( ((int)propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }
            
            return positionsOpenData( numItems * 3 );
        }
        else if ( ((int)propertyData) == NORMALS_NORMAL_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "normals.normal" );
            }
            
            return normalValuesOpenData( numItems * 3 );
        }
    }
    else if ( rp == READER_CLOSE )
    {
        if ( ((int)propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }
            
            return positionsCloseData( numItems * 3 );
        }
        else if ( ((int)propertyData) == NORMALS_NORMAL_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "normals.normal" );
            }

            return normalValuesCloseData( numItems * 3 );
        }
    }

    // INT
    if ( rp == READER_REF )
    {
        if ( ((int)propertyData) == INDICES_VERTEX_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "indices.vertex" );
            }

            return indicesData( numItems * 1 );
        }
        else if ( ((int)propertyData) == INDICES_ST_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "indices.st" );
            }

            return stIndicesData( numItems * 1 );
        }
        else if ( ((int)propertyData) == INDICES_NORMAL_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "indices.normal" );
            }

            return normalIndicesData( numItems * 1 );
        }
        else if ( ((int)propertyData) == SMOOTHING_METHOD_P )
        {
            if ( itemWidth != 1 ||
                 numItems != 1 ||
                 numBytes != sizeof( int ) )
            {
                WEIRD_SIZE( m_name, "smoothing.method" );
            }

            return ( void * )( &m_smoothingMethod );
        }
    }

    // UNSIGNED SHORT
    if ( rp == READER_REF )
    {
        if ( ((int)propertyData) == ELEMENTS_SIZE_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( unsigned short ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "elements.size" );
            }
            
            return numVertsData( numItems * 1 );
        }
    }

    // Superclass
    return Object::data( componentData, propertyData,
                         numItems, itemWidth, numBytes, rp );
}

//******************************************************************************
void Poly::dataRead( void *componentData,
                     void *propertyData,
                     ReaderPhase rp )
{
    if ( rp == READER_REF )
    {
        if ( ((int)propertyData) == ELEMENTS_SIZE_P )
        {
            delete[] m_numVertsInt;
            m_numVertsInt = new int[m_numVertsSize];
            for ( size_t i = 0; i < m_numVertsSize; ++i )
            {
                m_numVertsInt[i] = m_numVerts[i];
            }
            delete[] m_numVerts;
            m_numVerts = NULL;
        }
    }
}

//******************************************************************************
//******************************************************************************
// INTERNAL DATA STUFF
//******************************************************************************
//******************************************************************************
void *Poly::numVertsData( size_t numVertsSize )
{
    delete[] m_numVerts;
    delete[] m_numVertsInt;
    m_numVertsInt = NULL;
    m_numVertsSize = numVertsSize;
    m_numVerts = new unsigned short[m_numVertsSize];
    return ( void * )m_numVerts;
}

//******************************************************************************
void *Poly::indicesData( size_t indicesSize )
{
    delete[] m_indices;
    m_indicesSize = indicesSize;
    m_indices = new int[m_indicesSize];
    return ( void * )m_indices; 
}

//******************************************************************************
void *Poly::positionsRefData( size_t positionsRefSize )
{
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    m_positionsOpen = NULL;
    delete[] m_positionsClose;
    m_positionsClose = NULL;
    
    m_positionsSize = positionsRefSize;
    m_positionsRef = new float[m_positionsSize];
    return ( void * )m_positionsRef;
}

//******************************************************************************
void *Poly::positionsOpenData( size_t positionsOpenSize )
{
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    m_positionsClose = NULL;

    if ( positionsOpenSize != m_positionsSize ||
         m_positionsRef == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception,
                            "Shutter open positions for " << m_name
                            << " are incompatible\n"
                            "with reference positions (wrong size)\n"
                            "Reference model and open model are probably\n"
                            "not the same object\n" );
    }
    
    m_positionsOpen = new float[m_positionsSize];
    return ( void * )m_positionsOpen;
}

//******************************************************************************
void *Poly::positionsCloseData( size_t positionsCloseSize )
{
    delete[] m_positionsClose;

    if ( positionsCloseSize != m_positionsSize ||
         m_positionsRef == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception,
                            "Shutter close positions for " << m_name
                            << " are incompatible\n"
                            "with reference positions (wrong size)\n"
                            "Reference model and open model are probably\n"
                            "not the same object\n" );
    }
    
    m_positionsClose = new float[m_positionsSize];
    return ( void * )m_positionsClose;
}

//******************************************************************************
void *Poly::stValuesData( size_t stValuesSize )
{
    delete[] m_stValues;

    m_stValuesSize = stValuesSize;
    m_stValues = new float[m_stValuesSize];
    return ( void * )m_stValues;
}

//******************************************************************************
void *Poly::stIndicesData( size_t stIndicesSize )
{
    delete[] m_stIndices;

    m_stIndicesSize = stIndicesSize;
    m_stIndices = new int[m_stIndicesSize];
    return ( void * )m_stIndices;
}

// *****************************************************************************
void *Poly::normalValuesRefData( size_t normValuesSize )
{
    delete[] m_normalValuesRef;
    delete[] m_normalValuesOpen;
    m_normalValuesOpen = NULL;
    delete[] m_normalValuesClose;
    m_normalValuesClose = NULL;

    m_normalValuesSize = normValuesSize;
    m_normalValuesRef = new float[m_normalValuesSize];
    return ( void * )m_normalValuesRef;
}

// *****************************************************************************
void *Poly::normalValuesOpenData( size_t normValuesSize )
{
    delete[] m_normalValuesOpen;
    delete[] m_normalValuesClose;
    m_normalValuesClose = NULL;

    if ( m_normalValuesSize != normValuesSize )
    {
        std::cerr << "WARNING: shutter-open " <<  m_name
                  << " had different normal count"
                  << " than reference gto." << std::endl;
        m_normalValuesOpen = NULL;
        return NULL;
    }

    m_normalValuesOpen = new float[m_normalValuesSize];
    return ( void * )m_normalValuesOpen;
}

// *****************************************************************************
void *Poly::normalValuesCloseData( size_t normValuesSize )
{
    delete[] m_normalValuesClose;

    if ( m_normalValuesSize != normValuesSize )
    {
        std::cerr << "WARNING: shutter-close " <<  m_name
                  << " had different normal count"
                  << " than reference gto." << std::endl;
        m_normalValuesClose = NULL;
        return NULL;
    }

    m_normalValuesClose = new float[m_normalValuesSize];
    return ( void * )m_normalValuesClose;
}

// *****************************************************************************
void *Poly::normalIndicesData( size_t normIndicesSize )
{
    delete[] m_normalIndices;

    m_normalIndicesSize = normIndicesSize;
    m_normalIndices = new int[m_normalIndicesSize];
    return ( void * )m_normalIndices;
}

//******************************************************************************
namespace {
typedef float V2[2];
typedef float V3[3];
} // End anonymous namespace

//******************************************************************************
void Poly::internalDeclareRi() const
{
    // Validate
    int numVertices = 0;
    for ( int i = 0; i < m_numVertsSize; ++i )
    {
        numVertices += m_numVertsInt[i];
    }
    
    int maxVertexIndex = -1;
    for ( int i = 0; i < numVertices; ++i )
    {
        if ( m_indices[i] > maxVertexIndex )
        {
            maxVertexIndex = m_indices[i];
        }
    }
    
    if( m_smoothingMethod != GTO_SMOOTHING_METHOD_NONE
        && m_smoothingMethod != GTO_SMOOTHING_METHOD_PARTITIONED )
    {
        std::cerr << "WARNING: Unsupported smoothing method: ";
        std::cerr << m_smoothingMethod << ", using smoothing 0 (None)";
        std::cerr << std::endl;
    }
    
    // Need to make sts, if desired
    float *sValues = NULL;
    float *tValues = NULL;
    if ( m_stValuesSize > 0 && m_stValues != NULL &&
         m_stIndicesSize > 0 && m_stIndices != NULL )
    {
        const V2 *stValues = ( const V2 * )m_stValues;

        // There are as many sts as there are indices
        if ( m_indicesSize != m_stIndicesSize )
        {
            std::cerr << "WARNING: Invalid ST mapping on object "
                      << m_name.c_str() << std::endl
                      << "STs will not be set on surface" << std::endl;
        }
        else
        {
            sValues = new float[m_stIndicesSize];
            tValues = new float[m_stIndicesSize];
            
            for ( int i = 0; i < m_stIndicesSize; ++i )
            {
                int index = m_stIndices[i];
                if ( index < 0 || index >= ( m_stValuesSize / 2 ) )
                {
                    std::cerr << "WARNING: Invalid ST mapping on object "
                              << m_name.c_str() << std::endl
                              << "STs will not be set on surface" << std::endl;
                    delete[] sValues;
                    delete[] tValues;
                    sValues = NULL;
                    tValues = NULL;
                    break;
                }
                else
                {
                    sValues[i] = stValues[index][0];
                    tValues[i] = stValues[index][1];
                }
            }
        }
    }

    // Need to make normals, if desired
    float *normalValuesRef = NULL;
    float *normalValuesOpen = NULL;
    float *normalValuesClose = NULL;

    if ( m_normalValuesSize > 0 && m_normalValuesRef != NULL
         && m_normalIndicesSize > 0 && m_normalIndices != NULL
         && m_smoothingMethod == GTO_SMOOTHING_METHOD_PARTITIONED )
    {
        normalValuesRef = new float[m_normalIndicesSize * 3];
        
        for( int i = 0; i < m_normalIndicesSize; ++i )
        {
            int normalIndex = m_normalIndices[i];
            normalValuesRef[(i*3)+0] = m_normalValuesRef[(normalIndex*3)+0];
            normalValuesRef[(i*3)+1] = m_normalValuesRef[(normalIndex*3)+1];
            normalValuesRef[(i*3)+2] = m_normalValuesRef[(normalIndex*3)+2];
        }

        if( m_normalValuesOpen != NULL )
        {
            normalValuesOpen = new float[m_normalIndicesSize * 3];

            for( int i = 0; i < m_normalIndicesSize; ++i )
            {
                assert( m_normalIndices != NULL );
                int normalIndex = m_normalIndices[i];
                normalValuesOpen[(i*3)+0] = m_normalValuesOpen[(normalIndex*3)+0];
                normalValuesOpen[(i*3)+1] = m_normalValuesOpen[(normalIndex*3)+1];
                normalValuesOpen[(i*3)+2] = m_normalValuesOpen[(normalIndex*3)+2];
            }
        }
        else
        {
            normalValuesOpen = normalValuesRef;
        }

        if( m_normalValuesClose != NULL )
        {
            normalValuesClose = new float[m_normalIndicesSize * 3];

            assert( m_normalIndices != NULL );
            for( int i = 0; i < m_normalIndicesSize; ++i )
            {
                int normalIndex = m_normalIndices[i];
                normalValuesClose[(i*3)+0] = m_normalValuesClose[(normalIndex*3)+0];
                normalValuesClose[(i*3)+1] = m_normalValuesClose[(normalIndex*3)+1];
                normalValuesClose[(i*3)+2] = m_normalValuesClose[(normalIndex*3)+2];
            }
        }
        else
        {
            normalValuesClose = normalValuesRef;
        }
    }
   
    if ( m_indicesSize != numVertices ||
         m_positionsSize < ( maxVertexIndex + 1 ) * 3 )
    {
        std::cerr << "Invalid Poly surface: " << m_name << std::endl;
        return;
    }

    if ( m_positionsOpen == NULL )
    {
        // Reference positions only
        Stub_DeclarePoly( m_numVertsInt,
                          m_numVertsSize,
                          m_indices,
                          m_positionsRef,
                          m_positionsRef,
                          sValues, tValues,
                          normalValuesRef );
    }
    else
    {
        if ( m_positionsClose != NULL )
        {
            Stub_RiMotionBegin( 0.0f, 1.0f );

            Stub_DeclarePoly( m_numVertsInt,
                              m_numVertsSize,
                              m_indices,
                              m_positionsOpen,
                              m_positionsRef,
                              sValues, tValues,
                              normalValuesOpen  );

            Stub_DeclarePoly( m_numVertsInt,
                              m_numVertsSize,
                              m_indices,
                              m_positionsClose,
                              m_positionsRef,
                              sValues, tValues,
                              normalValuesClose  );
        
            Stub_RiMotionEnd();
        }
        else
        {
            Stub_DeclarePoly( m_numVertsInt,
                              m_numVertsSize,
                              m_indices,
                              m_positionsOpen,
                              m_positionsRef,
                              sValues, tValues ,
                              normalValuesOpen );
        }
    }
    
    delete[] sValues;
    delete[] tValues;
    delete[] normalValuesRef;
    if( normalValuesOpen != normalValuesRef )
    {
        delete[] normalValuesOpen;
    }
    if( normalValuesClose != normalValuesRef )
    {
        delete[] normalValuesClose;
    }
}

} // End namespace RiGto
