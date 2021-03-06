/* Copyright (c) 2011-2014, Cyrille Favreau
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille_favreau@hotmail.com>
 *
 * This file is part of Sol-R <https://github.com/cyrillefavreau/Sol-R>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma warning(disable : 4996)

#include "types.h"
#include "Logging.h"
#include "RayTracingEngineStub.h"
#include "engines/GPUKernel.h"
#include "io/FileMarshaller.h"
#include "io/OBJReader.h"
#include "io/PDBReader.h"

#include <fstream>

#ifdef USE_OPENCL
#include <engines/opencl/OpenCLKernel.h>
#endif

SceneInfo gSceneInfoStub;
PostProcessingInfo gPostProcessingInfoStub;

// --------------------------------------------------------------------------------
// Implementation
// --------------------------------------------------------------------------------
int RayTracer_SetSceneInfo(int width, int height, int graphicsLevel, int nbRayIterations, double transparentColor,
                           double viewDistance, double shadowIntensity, int renderingType, double width3DVision,
                           double bgColorR, double bgColorG, double bgColorB, double bgColorA, int renderBoxes,
                           int pathTracingIteration, int maxPathTracingIterations, int outputType, int timer,
                           int fogEffect, int isometric3D, int doubleSidedTriangles, int gradientBackGround,
                           int globalIllumination, int skyboxSize, int skyboxMaterialId, double geometryEpsilon,
                           double rayEpsilon)
{
    LOG_INFO(3, "RayTracer_SetSceneInfo ("
                    << width << "," << height << "," << graphicsLevel << "," << nbRayIterations << ","
                    << transparentColor << "," << viewDistance << "," << shadowIntensity << "," << width3DVision << ","
                    << bgColorR << "," << bgColorG << "," << bgColorB << "," << bgColorA << "," << renderingType << ","
                    << renderBoxes << "," << pathTracingIteration << "," << maxPathTracingIterations << ","
                    << outputType << "," << timer << "," << fogEffect << "," << isometric3D << "," << geometryEpsilon
                    << "," << rayEpsilon);

    gSceneInfoStub.size.x = width;
    gSceneInfoStub.size.y = height;
    gSceneInfoStub.graphicsLevel = graphicsLevel;
    gSceneInfoStub.nbRayIterations = nbRayIterations;
    gSceneInfoStub.transparentColor = static_cast<float>(transparentColor);
    gSceneInfoStub.viewDistance = static_cast<float>(viewDistance);
    gSceneInfoStub.shadowIntensity = static_cast<float>(shadowIntensity);
    gSceneInfoStub.width3DVision = static_cast<float>(width3DVision);
    gSceneInfoStub.backgroundColor.x = static_cast<float>(bgColorR);
    gSceneInfoStub.backgroundColor.y = static_cast<float>(bgColorG);
    gSceneInfoStub.backgroundColor.z = static_cast<float>(bgColorB);
    gSceneInfoStub.backgroundColor.w = static_cast<float>(bgColorA);
    gSceneInfoStub.renderingType = renderingType;
    gSceneInfoStub.renderBoxes = static_cast<int>(renderBoxes);
    gSceneInfoStub.pathTracingIteration = pathTracingIteration;
    gSceneInfoStub.maxPathTracingIterations = maxPathTracingIterations;
    gSceneInfoStub.misc.x = outputType;
    gSceneInfoStub.misc.y = timer;
    gSceneInfoStub.misc.z = fogEffect;
    gSceneInfoStub.misc.w = isometric3D;
    gSceneInfoStub.parameters.x = doubleSidedTriangles;
    gSceneInfoStub.parameters.y = gradientBackGround;
    gSceneInfoStub.parameters.z = globalIllumination;
    // gSceneInfoStub.parameters.w              = draftMode;

    gSceneInfoStub.skybox.x = skyboxSize;
    gSceneInfoStub.skybox.y = skyboxMaterialId;
    gSceneInfoStub.epsilon.x = geometryEpsilon;
    gSceneInfoStub.epsilon.y = rayEpsilon;
    return 0;
}

int RayTracer_SetPostProcessingInfo(int type, double param1, double param2, int param3)
{
    LOG_INFO(3, "RayTracer_SetPostProcessingInfo");
    gPostProcessingInfoStub.type = type;
    gPostProcessingInfoStub.param1 = static_cast<float>(param1);
    gPostProcessingInfoStub.param2 = static_cast<float>(param2);
    gPostProcessingInfoStub.param3 = param3;
    return 0;
}

int RayTracer_SetDraftMode(int draft)
{
    gSceneInfoStub.parameters.w = draft;
    return 0;
}

int RayTracer_InitializeKernel(bool activeLogging, int platform, int device)
{
    LOG_INFO(3, "RayTracer_InitializeKernel");
    solr::GPUKernel *Kernel = solr::SingletonKernel::kernel();
    if (Kernel)
    {
        gSceneInfoStub.pathTracingIteration = 0;
        Kernel->setSceneInfo(gSceneInfoStub);
        Kernel->initBuffers();
        Kernel->setFrame(0);
        return 0;
    }
    return -1;
}

// --------------------------------------------------------------------------------
int RayTracer_FinalizeKernel()
{
    LOG_INFO(3, "RayTracer_FinalizeKernel");
    return 0;
}

// --------------------------------------------------------------------------------
int RayTracer_ResetKernel()
{
    LOG_INFO(3, "RayTracer_ResetKernel");
    solr::SingletonKernel::kernel()->resetAll();
    return 0;
}

// --------------------------------------------------------------------------------
void RayTracer_SetCamera(double eye_x, double eye_y, double eye_z, double dir_x, double dir_y, double dir_z,
                         double angle_x, double angle_y, double angle_z)
{
    LOG_INFO(3, "RayTracer_SetCamera");
    const vec3f eye = {static_cast<float>(eye_x), static_cast<float>(eye_y), static_cast<float>(eye_z)};
    const vec3f dir = {static_cast<float>(dir_x), static_cast<float>(dir_y), static_cast<float>(dir_z)};
    const vec4f angles = {static_cast<float>(angle_x), static_cast<float>(angle_y), static_cast<float>(angle_z),
                          6400.f};
    solr::SingletonKernel::kernel()->setCamera(eye, dir, angles);
}

// --------------------------------------------------------------------------------
int RayTracer_RunKernel(double timer, BitmapBuffer *image)
{
    LOG_INFO(3, "RayTracer_RunKernel");
    solr::SingletonKernel::kernel()->setSceneInfo(gSceneInfoStub);
    solr::SingletonKernel::kernel()->setPostProcessingInfo(gPostProcessingInfoStub);
    solr::SingletonKernel::kernel()->render_begin(static_cast<float>(timer));
    solr::SingletonKernel::kernel()->render_end();
    if (false && gSceneInfoStub.misc.x == otDelphi)
    {
        BitmapBuffer *dst = new BitmapBuffer[gSceneInfoStub.size.x * gSceneInfoStub.size.y * gColorDepth];
        BitmapBuffer *src = solr::SingletonKernel::kernel()->getBitmap();
        for (int y(0); y < gSceneInfoStub.size.y; ++y)
        {
            for (int x(0); x < gSceneInfoStub.size.x * gColorDepth; x += gColorDepth)
            {
                int indexSrc = (y * gSceneInfoStub.size.x * gColorDepth) + x;
                int indexDst = (y + 1) * (gSceneInfoStub.size.x * gColorDepth) - x - gColorDepth;
                dst[indexDst] = src[indexSrc];
                dst[indexDst + 1] = src[indexSrc + 1];
                dst[indexDst + 2] = src[indexSrc + 2];
            }
        }
        memcpy(image, dst, gSceneInfoStub.size.x * gSceneInfoStub.size.y * gColorDepth);
        delete[] dst;
    }
    else
    {
        memcpy(image, solr::SingletonKernel::kernel()->getBitmap(),
               gSceneInfoStub.size.x * gSceneInfoStub.size.y * gColorDepth);
    }
    return 0;
}

// --------------------------------------------------------------------------------
int RayTracer_AddPrimitive(int type, int movable)
{
    LOG_INFO(3, "RayTracer_AddPrimitive");
    int id = solr::SingletonKernel::kernel()->addPrimitive(static_cast<PrimitiveType>(type));
    solr::SingletonKernel::kernel()->setPrimitiveIsMovable(id, (movable == 1));
    return id;
}

// --------------------------------------------------------------------------------
int RayTracer_SetPrimitive(int index, double p0_x, double p0_y, double p0_z, double p1_x, double p1_y, double p1_z,
                           double p2_x, double p2_y, double p2_z, double size_x, double size_y, double size_z,
                           int materialId)
{
    LOG_INFO(3, "RayTracer_SetPrimitive");
    solr::SingletonKernel::kernel()->setPrimitive(index, static_cast<float>(p0_x), static_cast<float>(p0_y),
                                                  static_cast<float>(p0_z), static_cast<float>(p1_x),
                                                  static_cast<float>(p1_y), static_cast<float>(p1_z),
                                                  static_cast<float>(p2_x), static_cast<float>(p2_y),
                                                  static_cast<float>(p2_z), static_cast<float>(size_x),
                                                  static_cast<float>(size_y), static_cast<float>(size_z), materialId);
    return 0;
}

int RayTracer_GetPrimitive(int index, double &p0_x, double &p0_y, double &p0_z, double &p1_x, double &p1_y,
                           double &p1_z, double &p2_x, double &p2_y, double &p2_z, double &size_x, double &size_y,
                           double &size_z, int &materialId)
{
    LOG_INFO(3, "RayTracer_GetPrimitive");
    solr::CPUPrimitive *primitive = solr::SingletonKernel::kernel()->getPrimitive(index);
    if (primitive != NULL)
    {
        p0_x = primitive->p0.x;
        p0_y = primitive->p0.y;
        p0_z = primitive->p0.z;
        p1_x = primitive->p1.x;
        p1_y = primitive->p1.y;
        p1_z = primitive->p1.z;
        p2_x = primitive->p2.x;
        p2_y = primitive->p2.y;
        p2_z = primitive->p2.z;
        size_x = primitive->size.x;
        size_y = primitive->size.y;
        size_z = primitive->size.z;
        materialId = primitive->materialId;
        return 0;
    }
    return -1;
}

int RayTracer_GetPrimitiveAt(int x, int y)
{
    LOG_INFO(3, "RayTracer_GetPrimitiveAt");
    return solr::SingletonKernel::kernel()->getPrimitiveAt(x, y);
}

int RayTracer_GetPrimitiveCenter(int index, double &x, double &y, double &z)
{
    LOG_INFO(3, "RayTracer_GetPrimitiveCenter");
    vec4f center = solr::SingletonKernel::kernel()->getPrimitiveCenter(index);
    x = static_cast<double>(center.x);
    y = static_cast<double>(center.y);
    z = static_cast<double>(center.z);
    return 0;
}

int RayTracer_RotatePrimitive(int index, double rx, double ry, double rz, double ax, double ay, double az)
{
    LOG_INFO(3, "RayTracer_RotatePrimitive");
    vec4f rotationCenter = {static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz), 0.f};
    vec4f angles = {static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az), 0.f};

    // solr::SingletonKernel::kernel()->rotatePrimitive( index, boxId, rotationCenter, angles ); // TODO!!
    return 0;
}

int RayTracer_RotatePrimitives(int fromBoxId, int toBoxId, double rx, double ry, double rz, double ax, double ay,
                               double az)
{
    LOG_INFO(3, "RayTracer_RotatePrimitives");
    try
    {
        vec3f rotationCenter = {static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz)};
        vec4f angles = {static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az)};

        solr::SingletonKernel::kernel()->rotatePrimitives(rotationCenter, angles);
        solr::SingletonKernel::kernel()->compactBoxes(false);
    }
    catch (...)
    {
    }
    return 0;
}

int RayTracer_SetPrimitiveMaterial(int index, int materialId)
{
    LOG_INFO(3, "RayTracer_SetPrimitiveMaterial");
    solr::SingletonKernel::kernel()->setPrimitiveMaterial(index, materialId);
    return 0;
}

int RayTracer_GetPrimitiveMaterial(int index)
{
    LOG_INFO(3, "RayTracer_GetPrimitiveMaterial");
    return solr::SingletonKernel::kernel()->getPrimitiveMaterial(index);
}

int RayTracer_SetPrimitiveNormals(int index, double n0_x, double n0_y, double n0_z, double n1_x, double n1_y,
                                  double n1_z, double n2_x, double n2_y, double n2_z)
{
    LOG_INFO(3, "RayTracer_SetPrimitiveNormals");
    vec3f n0 = {static_cast<float>(n0_x), static_cast<float>(n0_y), static_cast<float>(n0_z)};
    vec3f n1 = {static_cast<float>(n1_x), static_cast<float>(n1_y), static_cast<float>(n1_z)};
    vec3f n2 = {static_cast<float>(n2_x), static_cast<float>(n2_y), static_cast<float>(n2_z)};
    solr::SingletonKernel::kernel()->setPrimitiveNormals(index, n0, n1, n2);
    return 0;
}

int RayTracer_SetPrimitiveTextureCoordinates(int index, double t0_x, double t0_y, double t0_z, double t1_x, double t1_y,
                                             double t1_z, double t2_x, double t2_y, double t2_z)
{
    LOG_INFO(3, "RayTracer_SetPrimitiveTextureCoordinates");
    vec3f t0 = {static_cast<float>(t0_x), static_cast<float>(t0_y), static_cast<float>(t0_z)};
    vec3f t1 = {static_cast<float>(t1_x), static_cast<float>(t1_y), static_cast<float>(t1_z)};
    vec3f t2 = {static_cast<float>(t2_x), static_cast<float>(t2_y), static_cast<float>(t2_z)};
    solr::SingletonKernel::kernel()->setPrimitiveTextureCoordinates(index, t0, t1, t2);
    return 0;
}

// --------------------------------------------------------------------------------
int RayTracer_UpdateSkeletons(int index, double p0_x, double p0_y, double p0_z, double size, double radius,
                              int materialId, double head_radius, int head_materialId, double hands_radius,
                              int hands_materialId, double feet_radius, int feet_materialId)
{
    LOG_INFO(3, "RayTracer_UpdateSkeletons");
#if USE_KINECT
    vec3f position = {static_cast<float>(p0_x), static_cast<float>(p0_y), static_cast<float>(p0_z)};
    return solr::SingletonKernel::kernel()->updateSkeletons(index, position, static_cast<float>(size),
                                                            static_cast<float>(radius), materialId,
                                                            static_cast<float>(head_radius), head_materialId,
                                                            static_cast<float>(hands_radius), hands_materialId,
                                                            static_cast<float>(feet_radius), feet_materialId);
#else
    return 0;
#endif // USE_KINECT
}

// --------------------------------------------------------------------------------
int RayTracer_LoadTextureFromFile(int index, char *filename)
{
    LOG_INFO(3, "RayTracer_LoadTextureFromFile");
    return solr::SingletonKernel::kernel()->loadTextureFromFile(index, filename);
}

// --------------------------------------------------------------------------------
int RayTracer_SetTexture(int index, HANDLE texture)
{
    LOG_INFO(3, "RayTracer_SetTexture");
    /* TODO
 CPUTextureInformation texInfo;
 solr::SingletonKernel::kernel()->setTexture(
    index,
    static_cast<char*>(texture) );
 */
    return 0;
}

int RayTracer_GetTextureSize(int index, int &width, int &height, int &depth)
{
    LOG_INFO(3, "RayTracer_GetTextureSize");
    if (index < static_cast<int>(solr::SingletonKernel::kernel()->getNbActiveTextures()))
    {
        TextureInformation texInfo;
        memset(&texInfo, 0, sizeof(TextureInformation));
        solr::SingletonKernel::kernel()->getTexture(index, texInfo);
        if (texInfo.buffer)
        {
            width = texInfo.size.x;
            height = texInfo.size.y;
            depth = texInfo.size.z;
        }
        return 0;
    }
    return 1;
}

int RayTracer_GetTexture(int index, BitmapBuffer *image)
{
    LOG_INFO(3, "RayTracer_GetTexture");
    if (index < static_cast<int>(solr::SingletonKernel::kernel()->getNbActiveTextures()))
    {
        TextureInformation texInfo;
        memset(&texInfo, 0, sizeof(TextureInformation));
        solr::SingletonKernel::kernel()->getTexture(index, texInfo);
        if (texInfo.buffer)
        {
            int len(texInfo.size.x * texInfo.size.y * texInfo.size.z);
            for (int i(0); i < len; i += texInfo.size.z)
            {
                image[i] = texInfo.buffer[i + 2];
                image[i + 1] = texInfo.buffer[i + 1];
                image[i + 2] = texInfo.buffer[i];
            }
            // memcpy(image,texInfo.buffer,texInfo.size.x*texInfo.size.y*texInfo.size.z);
        }
        return 0;
    }
    return 1;
}

int RayTracer_GetNbTextures(int &nbTextures)
{
    LOG_INFO(3, "RayTracer_GetNbTextures");
    return solr::SingletonKernel::kernel()->getNbActiveTextures();
}

// ---------- Materials ----------
int RayTracer_AddMaterial()
{
    LOG_INFO(3, "RayTracer_AddMaterial");
    return solr::SingletonKernel::kernel()->addMaterial();
}

// --------------------------------------------------------------------------------
int RayTracer_SetMaterial(int index, double color_r, double color_g, double color_b, double noise, double reflection,
                          double refraction, int procedural, int wireframe, int wireframeDepth, double transparency,
                          double opacity, int diffuseTextureId, int normalTextureId, int bumpTextureId,
                          int specularTextureId, int reflectionTextureId, int transparencyTextureId,
                          int ambientOcclusionTextureId, double specValue, double specPower, double specCoef,
                          double innerIllumination, double illuminationDiffusion, double illuminationPropagation,
                          int fastTransparency)
{
    LOG_INFO(3, "RayTracer_SetMaterial ("
                    << index << "," << static_cast<float>(color_r) << "," << static_cast<float>(color_g) << ","
                    << static_cast<float>(color_b) << "," << static_cast<float>(noise) << ","
                    << static_cast<float>(reflection) << "," << static_cast<float>(refraction) << "," << procedural
                    << "," << wireframe << "," << static_cast<int>(wireframeDepth) << ","
                    << static_cast<float>(transparency) << "," << static_cast<float>(opacity) << ","
                    << static_cast<int>(diffuseTextureId) << "," << static_cast<int>(normalTextureId) << ","
                    << static_cast<int>(bumpTextureId) << "," << static_cast<int>(specularTextureId) << ","
                    << static_cast<int>(reflectionTextureId) << "," << static_cast<int>(transparencyTextureId) << ","
                    << static_cast<int>(ambientOcclusionTextureId) << "," << static_cast<float>(specValue) << ","
                    << static_cast<float>(specPower) << "," << static_cast<float>(specCoef) << ","
                    << static_cast<float>(innerIllumination) << "," << static_cast<float>(illuminationDiffusion) << ","
                    << static_cast<float>(illuminationPropagation) << "," << fastTransparency << ")");

    solr::SingletonKernel::kernel()->setMaterial(
        index, static_cast<float>(color_r), static_cast<float>(color_g), static_cast<float>(color_b),
        static_cast<float>(noise), static_cast<float>(reflection), static_cast<float>(refraction), (procedural == 1),
        (wireframe == 1), static_cast<int>(wireframeDepth), static_cast<float>(transparency),
        static_cast<float>(opacity), static_cast<int>(diffuseTextureId), static_cast<int>(normalTextureId),
        static_cast<int>(bumpTextureId), static_cast<int>(specularTextureId), static_cast<int>(reflectionTextureId),
        static_cast<int>(transparencyTextureId), static_cast<int>(ambientOcclusionTextureId),
        static_cast<float>(specValue), static_cast<float>(specPower), static_cast<float>(specCoef),
        static_cast<float>(innerIllumination), static_cast<float>(illuminationDiffusion),
        static_cast<float>(illuminationPropagation), (fastTransparency == 1));
    return 0;
}

// --------------------------------------------------------------------------------
int RayTracer_GetMaterial(int in_index, double &out_color_r, double &out_color_g, double &out_color_b,
                          double &out_noise, double &out_reflection, double &out_refraction, int &out_procedural,
                          int &out_wireframe, int &out_wireframeDepth, double &out_transparency, double &out_opacity,
                          int &out_diffuseTextureId, int &out_normalTextureId, int &out_bumpTextureId,
                          int &out_specularTextureId, int &out_reflectionTextureId, int &out_transparencyTextureId,
                          int &out_ambientOcclusionTextureId, double &out_specValue, double &out_specPower,
                          double &out_specCoef, double &out_innerIllumination, double &out_illuminationDiffusion,
                          double &out_illuminationPropagation, int &out_fastTransparency)
{
    LOG_INFO(3, "RayTracer_GetMaterial");
    float color_r, color_g, color_b, noise, reflection, refraction, transparency, opacity;
    float specValue, specPower, specCoef, innerIllumination, illuminationDiffusion, illuminationPropagation;
    bool procedural;
    bool wireframe;
    int wireframeDepth;
    int diffuseTextureId, normalTextureId, bumpTextureId, specularTextureId, reflectionTextureId, transparencyTextureId,
        ambientOcclusionTextureId;
    bool fastTransparency;

    int returnValue = solr::SingletonKernel::kernel()->getMaterialAttributes(
        in_index, color_r, color_g, color_b, noise, reflection, refraction, procedural, wireframe, wireframeDepth,
        transparency, opacity, diffuseTextureId, normalTextureId, bumpTextureId, specularTextureId, reflectionTextureId,
        transparencyTextureId, ambientOcclusionTextureId, specValue, specPower, specCoef, innerIllumination,
        illuminationDiffusion, illuminationPropagation, fastTransparency);

    out_color_r = static_cast<double>(color_r);
    out_color_g = static_cast<double>(color_g);
    out_color_b = static_cast<double>(color_b);
    out_noise = static_cast<double>(noise);
    out_reflection = static_cast<double>(reflection);
    out_refraction = static_cast<double>(refraction);
    out_transparency = static_cast<double>(transparency);
    out_opacity = static_cast<double>(opacity);
    out_diffuseTextureId = static_cast<int>(diffuseTextureId);
    out_normalTextureId = static_cast<int>(normalTextureId);
    out_bumpTextureId = static_cast<int>(bumpTextureId);
    out_specularTextureId = static_cast<int>(specularTextureId);
    out_reflectionTextureId = static_cast<int>(reflectionTextureId);
    out_transparencyTextureId = static_cast<int>(transparencyTextureId);
    out_ambientOcclusionTextureId = static_cast<int>(ambientOcclusionTextureId);
    out_procedural = procedural ? 1 : 0;
    out_wireframe = wireframe ? 1 : 0;
    out_wireframeDepth = wireframeDepth;
    out_specValue = static_cast<double>(specValue);
    out_specPower = static_cast<double>(specPower);
    out_specCoef = static_cast<double>(specCoef);
    out_innerIllumination = static_cast<double>(innerIllumination);
    out_illuminationDiffusion = static_cast<double>(illuminationDiffusion);
    out_illuminationPropagation = static_cast<double>(illuminationPropagation);
    out_fastTransparency = fastTransparency ? 1 : 0;
    return returnValue;
}

// --------------------------------------------------------------------------------
int RayTracer_LoadMolecule(char *filename, int geometryType, double defaultAtomSize, double defaultStickSize,
                           int atomMaterialType, double scale)
{
    LOG_INFO(3, "RayTracer_LoadMolecule");
    // PDB
    solr::PDBReader prbReader;
    float s(static_cast<float>(scale));
    vec4f objectScale = {s, s, s};
    vec4f minPos =
        prbReader.loadAtomsFromFile(filename, *solr::SingletonKernel::kernel(),
                                    static_cast<solr::GeometryType>(geometryType), static_cast<float>(defaultAtomSize),
                                    static_cast<float>(defaultStickSize), atomMaterialType, objectScale);
    return solr::SingletonKernel::kernel()->getNbActivePrimitives();
}

// --------------------------------------------------------------------------------
int RayTracer_LoadOBJModel(char *filename, int materialId, int autoScale, double scale, int autoCenter, double &height)
{
    LOG_INFO(3, "RayTracer_LoadOBJModel");
    vec4f center = {0.f, 0.f, 0.f};
    // PDB
    solr::OBJReader objReader;
    float s(static_cast<float>(scale));
    vec4f objectScale = {s, s, s};
    solr::CPUBoundingBox aabb;
    solr::CPUBoundingBox inAABB;
    vec4f minPos =
        objReader.loadModelFromFile(filename, *solr::SingletonKernel::kernel(), center, (autoScale == 1), objectScale,
                                    true, materialId, false, (autoCenter == 1), aabb, false, inAABB);

    height = -minPos.y / 2.f;
    return solr::SingletonKernel::kernel()->getNbActivePrimitives();
}

// --------------------------------------------------------------------------------
int RayTracer_SaveToFile(char *filename)
{
    LOG_INFO(3, "RayTracer_SaveToFile");
    solr::FileMarshaller fm;
    fm.saveToFile(*solr::SingletonKernel::kernel(), filename);
    return solr::SingletonKernel::kernel()->getNbActivePrimitives();
}

// --------------------------------------------------------------------------------
int RayTracer_LoadFromFile(char *filename, double scale)
{
    LOG_INFO(3, "RayTracer_LoadFromFile");
    vec4f center = {0.f, 0.f, 0.f};
    solr::FileMarshaller fm;
    fm.loadFromFile(*solr::SingletonKernel::kernel(), filename, center, static_cast<float>(scale));
    return solr::SingletonKernel::kernel()->getNbActivePrimitives();
}

// --------------------------------------------------------------------------------
int RayTracer_CompactBoxes(bool update)
{
    LOG_INFO(3, "RayTracer_CompactBoxes");
    return solr::SingletonKernel::kernel()->compactBoxes(update);
}

// --------------------------------------------------------------------------------
int RayTracer_GetLight(int index)
{
    LOG_INFO(3, "RayTracer_GetLight");
    return solr::SingletonKernel::kernel()->getLight(index);
}

int RayTracer_GenerateScreenshot(char *filename, int width, int height, int quality)
{
    solr::SingletonKernel::kernel()->generateScreenshot(filename, width, height, quality);
    return 0;
}

#ifdef USE_OPENCL
int RayTracer_GetOpenCLPlaformCount()
{
    return dynamic_cast<solr::OpenCLKernel *>(solr::SingletonKernel::kernel())->getNumPlatforms();
}

int RayTracer_GetOpenCLPlatformDescription(int platform, char *value, int valueLength)
{
    std::string description =
        dynamic_cast<solr::OpenCLKernel *>(solr::SingletonKernel::kernel())->getPlatformDescription(platform);
    strncpy(value, description.c_str(), valueLength);
    return 0;
}

int RayTracer_GetOpenCLDeviceCount(int platform)
{
    return dynamic_cast<solr::OpenCLKernel *>(solr::SingletonKernel::kernel())->getNumDevices(platform);
}

int RayTracer_GetOpenCLDeviceDescription(int platform, int device, char *value, int valueLength)
{
    std::string description =
        dynamic_cast<solr::OpenCLKernel *>(solr::SingletonKernel::kernel())->getDeviceDescription(platform, device);
    strncpy(value, description.c_str(), valueLength);
    return 0;
}

int RayTracer_PopulateOpenCLInformation()
{
    dynamic_cast<solr::OpenCLKernel *>(solr::SingletonKernel::kernel())->queryDevice();
    return 0;
}

int RayTracer_RecompileKernels(char *filename)
{
    solr::SingletonKernel::kernel()->setKernelFilename(filename);
    solr::SingletonKernel::kernel()->recompileKernels();
    return 0;
}
#else
int RayTracer_GetOpenCLPlaformCount()
{
    return 0;
}

int RayTracer_GetOpenCLPlatformDescription(int platform, char *value, int valueLength)
{
    std::string description = "This library was not compiled for OpenCL";
    strncpy(value, description.c_str(), valueLength);
    return 0;
}

int RayTracer_GetOpenCLDeviceCount(int platform)
{
    return 0;
}

int RayTracer_GetOpenCLDeviceDescription(int platform, int device, char *value, int valueLength)
{
    std::string description = "This library was not compiled for OpenCL";
    strncpy(value, description.c_str(), valueLength);
    return 0;
}

int RayTracer_PopulateOpenCLInformation()
{
    return 0;
}

int RayTracer_RecompileKernels(char *)
{
    return 0;
}
#endif // USE_OPENCL
