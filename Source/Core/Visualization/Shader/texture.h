/*****************************************************************************/
/**
 *  @file   texture.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/

#if __VERSION__ >= 130
#define LookupTexture1D( sampler, coord ) \
    texture( sampler, coord )
#else
#define LookupTexture1D( sampler, coord ) \
    texture1D( sampler, coord )
#endif

#if __VERSION__ >= 130
#define LookupTexture2D( sampler, coord ) \
    texture( sampler, coord )
#else
#define LookupTexture2D( sampler, coord ) \
    texture2D( sampler, coord )
#endif

#if __VERSION__ >= 130
#define LookupTexture3D( sampler, coord ) \
    texture( sampler, coord )
#else
#define LookupTexture3D( sampler, coord ) \
    texture3D( sampler, coord )
#endif
