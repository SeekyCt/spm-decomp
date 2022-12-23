#pragma once

#include <common.h>

#define GX_FALSE			0
#define GX_TRUE				1
#define GX_DISABLE			0
#define GX_ENABLE			1

/*! \addtogroup clipmode Clipping mode
 * @{
 */
#define GX_CLIP_DISABLE		1
#define GX_CLIP_ENABLE		0
/*! @} */

#define GX_FIFO_MINSIZE		(64*1024)			/*!< Smallest usable graphics FIFO size. */
#define GX_FIFO_HIWATERMARK	(16*1024)			/*!< Default hi watermark for FIFO buffer control. */
#define GX_FIFO_OBJSIZE		128

#define GX_PERSPECTIVE		0
#define GX_ORTHOGRAPHIC		1

#define GX_MT_NULL			0
#define GX_MT_XF_FLUSH		1
#define GX_MT_DL_SAVE_CTX	2

#define GX_XF_FLUSH_NONE	0
#define GX_XF_FLUSH_SAFE	1

/*! \addtogroup channelid Color channel ID
 * @{
 */
#define GX_COLOR0			0
#define GX_COLOR1			1
#define GX_ALPHA0			2
#define GX_ALPHA1			3
#define GX_COLOR0A0			4
#define GX_COLOR1A1			5
#define GX_COLORZERO		6
#define GX_ALPHA_BUMP		7
#define GX_ALPHA_BUMPN		8
#define GX_COLORNULL		0xff
/*! @} */

/*! \addtogroup mtxtype Matrix type
 * @{
 */
#define GX_MTX3x4			0
#define GX_MTX2x4			1
/*! @} */

/*! \addtogroup vtxfmt Vertex format index
 * @{
 */
#define GX_VTXFMT0			0
#define GX_VTXFMT1			1
#define GX_VTXFMT2			2
#define GX_VTXFMT3			3
#define GX_VTXFMT4			4
#define GX_VTXFMT5			5
#define GX_VTXFMT6			6
#define GX_VTXFMT7			7
#define GX_MAXVTXFMT		8

/*! @} */


/*! \addtogroup vtxattrin Vertex data input type
 * @{
 */
#define GX_NONE				0			/*!< Input data is not used */
#define GX_DIRECT			1			/*!< Input data is set direct */
#define GX_INDEX8			2			/*!< Input data is set by a 8bit index */
#define GX_INDEX16			3			/*!< Input data is set by a 16bit index */

/*! @} */


/*! \addtogroup compsize Number of components in an attribute
 * @{
 */
#define GX_U8				0			/*!< Unsigned 8-bit integer */
#define GX_S8				1			/*!< Signed 8-bit integer */
#define GX_U16				2			/*!< Unsigned 16-bit integer */
#define GX_S16				3			/*!< Signed 16-bit integer */
#define GX_F32				4			/*!< 32-bit floating-point */
#define GX_RGB565			0			/*!< 16-bit RGB */
#define GX_RGB8				1			/*!< 24-bit RGB */
#define GX_RGBX8			2			/*!< 32-bit RGBX */
#define GX_RGBA4			3			/*!< 16-bit RGBA */
#define GX_RGBA6			4			/*!< 24-bit RGBA */
#define GX_RGBA8			5			/*!< 32-bit RGBA */
/*! @} */

/*! \addtogroup comptype Attribute component type
 * @{
 */
#define GX_POS_XY			0			/*!< X,Y position */
#define GX_POS_XYZ			1			/*!< X,Y,Z position */
#define GX_NRM_XYZ			0			/*!< X,Y,Z normal */
#define GX_NRM_NBT			1
#define GX_NRM_NBT3			2
#define GX_CLR_RGB			0			/*!< RGB color */
#define GX_CLR_RGBA			1			/*!< RGBA color */
#define GX_TEX_S			0			/*!< One texture dimension */
#define GX_TEX_ST			1			/*!< Two texture dimensions */
/*! @} */

/*! \addtogroup vtxattr Vertex attribute array type
 * @{
 */
#define GX_VA_PTNMTXIDX			0
#define GX_VA_TEX0MTXIDX		1
#define GX_VA_TEX1MTXIDX		2
#define GX_VA_TEX2MTXIDX		3
#define GX_VA_TEX3MTXIDX		4
#define GX_VA_TEX4MTXIDX		5
#define GX_VA_TEX5MTXIDX		6
#define GX_VA_TEX6MTXIDX		7
#define GX_VA_TEX7MTXIDX		8
#define GX_VA_POS				9
#define GX_VA_NRM				10
#define GX_VA_CLR0				11
#define GX_VA_CLR1				12
#define GX_VA_TEX0				13
#define GX_VA_TEX1				14
#define GX_VA_TEX2				15
#define GX_VA_TEX3				16
#define GX_VA_TEX4				17
#define GX_VA_TEX5				18
#define GX_VA_TEX6				19
#define GX_VA_TEX7				20
#define GX_POSMTXARRAY			21
#define GX_NRMMTXARRAY			22
#define GX_TEXMTXARRAY			23
#define GX_LIGHTARRAY			24
#define GX_VA_NBT				25
#define GX_VA_MAXATTR			26
#define GX_VA_NULL				0xff
/*! @} */

/*! \addtogroup primtype Primitive type
 * \brief Collection of primitive types that can be drawn by the GP.
 *
 * \note Which type you use depends on your needs; however, performance can increase by using triangle strips or fans instead of discrete triangles.
 * @{
 */
#define GX_POINTS				0xB8			/*!< Draws a series of points. Each vertex is a single point. */
#define GX_LINES				0xA8			/*!< Draws a series of unconnected line segments. Each pair of vertices makes a line. */
#define GX_LINESTRIP			0xB0			/*!< Draws a series of lines. Each vertex (besides the first) makes a line between it and the previous. */
#define GX_TRIANGLES			0x90			/*!< Draws a series of unconnected triangles. Three vertices make a single triangle. */
#define GX_TRIANGLESTRIP		0x98			/*!< Draws a series of triangles. Each triangle (besides the first) shares a side with the previous triangle.
												* Each vertex (besides the first two) completes a triangle. */
#define GX_TRIANGLEFAN			0xA0			/*!< Draws a single triangle fan. The first vertex is the "centerpoint". The second and third vertex complete
												* the first triangle. Each subsequent vertex completes another triangle which shares a side with the previous
												* triangle (except the first triangle) and has the centerpoint vertex as one of the vertices. */
#define GX_QUADS				0x80			/*!< Draws a series of unconnected quads. Every four vertices completes a quad. Internally, each quad is
												* translated into a pair of triangles. */
/*! @} */

#define GX_SRC_REG			0
#define GX_SRC_VTX			1

/*! \addtogroup lightid Light ID
 * @{
 */
#define GX_LIGHT0			0x001			/*!< Light 0 */
#define GX_LIGHT1			0x002			/*!< Light 2 */
#define GX_LIGHT2			0x004			/*!< Light 3 */
#define GX_LIGHT3			0x008			/*!< Light 4 */
#define GX_LIGHT4			0x010			/*!< Light 5 */
#define GX_LIGHT5			0x020			/*!< Light 6 */
#define GX_LIGHT6			0x040			/*!< Light 7 */
#define GX_LIGHT7			0x080			/*!< Light 8 */
#define GX_MAXLIGHT			0x100			/*!< All lights */
#define GX_LIGHTNULL		0x000			/*!< No lights */
/*! @} */

/*! \addtogroup difffn Diffuse function
 * @{
 */
#define GX_DF_NONE			0
#define GX_DF_SIGNED		1
#define GX_DF_CLAMP			2
/*! @} */

/*! \addtogroup attenfunc Attenuation function
 * @{
 */
#define GX_AF_SPEC			0			/*!< Specular computation */
#define GX_AF_SPOT			1			/*!< Spot light attenuation */
#define GX_AF_NONE			2			/*!< No attenuation */
/*! @} */

/* pos,nrm,tex,dtt matrix */
/*! \addtogroup pnmtx Position-normal matrix index
 * @{
 */
#define GX_PNMTX0			0
#define GX_PNMTX1			3
#define GX_PNMTX2			6
#define GX_PNMTX3			9
#define GX_PNMTX4			12
#define GX_PNMTX5			15
#define GX_PNMTX6			18
#define GX_PNMTX7			21
#define GX_PNMTX8			24
#define GX_PNMTX9			27
/*! @} */

/*! \addtogroup texmtx Texture matrix index
 * @{
 */
#define GX_TEXMTX0			30
#define GX_TEXMTX1			33
#define GX_TEXMTX2			36
#define GX_TEXMTX3			39
#define GX_TEXMTX4			42
#define GX_TEXMTX5			45
#define GX_TEXMTX6			48
#define GX_TEXMTX7			51
#define GX_TEXMTX8			54
#define GX_TEXMTX9			57
#define GX_IDENTITY			60
/*! @} */

/*! \addtogroup dttmtx Post-transform texture matrix index
 * @{
 */
#define GX_DTTMTX0			64
#define GX_DTTMTX1			67
#define GX_DTTMTX2			70
#define GX_DTTMTX3			73
#define GX_DTTMTX4			76
#define GX_DTTMTX5			79
#define GX_DTTMTX6			82
#define GX_DTTMTX7			85
#define GX_DTTMTX8			88
#define GX_DTTMTX9			91
#define GX_DTTMTX10			94
#define GX_DTTMTX11			97
#define GX_DTTMTX12			100
#define GX_DTTMTX13			103
#define GX_DTTMTX14			106
#define GX_DTTMTX15			109
#define GX_DTTMTX16			112
#define GX_DTTMTX17			115
#define GX_DTTMTX18			118
#define GX_DTTMTX19			121
#define GX_DTTIDENTITY		125
/*! @} */

/* tex coord id
   used by: XF: 0x1040,0x1050
            BP: 0x30
*/
/*! \addtogroup texcoordid texture coordinate slot
 * @{
 */
#define GX_TEXCOORD0		0x0
#define GX_TEXCOORD1		0x1
#define GX_TEXCOORD2		0x2
#define GX_TEXCOORD3		0x3
#define GX_TEXCOORD4		0x4
#define GX_TEXCOORD5		0x5
#define GX_TEXCOORD6		0x6
#define GX_TEXCOORD7		0x7
#define GX_MAXCOORD			0x8
#define GX_TEXCOORDNULL		0xff
/*! @} */

/* tex format */
#define _GX_TF_ZTF			0x10
#define _GX_TF_CTF			0x20

/*! \addtogroup texfmt Texture format
 * @{
 */

#define GX_TF_I4			0x0
#define GX_TF_I8			0x1
#define GX_TF_IA4			0x2
#define GX_TF_IA8			0x3
#define GX_TF_RGB565		0x4
#define GX_TF_RGB5A3		0x5
#define GX_TF_RGBA8			0x6
#define GX_TF_CI4			0x8
#define GX_TF_CI8			0x9
#define GX_TF_CI14			0xa
#define GX_TF_CMPR			0xE			/*!< Compressed */

#define GX_TL_IA8			0x00
#define GX_TL_RGB565		0x01
#define GX_TL_RGB5A3		0x02

#define GX_CTF_R4			(0x0|_GX_TF_CTF)			/*!< For copying 4 bits from red */
#define GX_CTF_RA4			(0x2|_GX_TF_CTF)			/*!< For copying 4 bits from red, 4 bits from alpha */
#define GX_CTF_RA8			(0x3|_GX_TF_CTF)			/*!< For copying 8 bits from red, 8 bits from alpha */
#define GX_CTF_YUVA8		(0x6|_GX_TF_CTF)
#define GX_CTF_A8			(0x7|_GX_TF_CTF)			/*!< For copying 8 bits from alpha */
#define GX_CTF_R8			(0x8|_GX_TF_CTF)			/*!< For copying 8 bits from red */
#define GX_CTF_G8			(0x9|_GX_TF_CTF)			/*!< For copying 8 bits from green */
#define GX_CTF_B8			(0xA|_GX_TF_CTF)			/*!< For copying 8 bits from blue */
#define GX_CTF_RG8			(0xB|_GX_TF_CTF)			/*!< For copying 8 bits from red, 8 bits from green */
#define GX_CTF_GB8			(0xC|_GX_TF_CTF)			/*!< For copying 8 bits from green, 8 bits from blue */

/*! \addtogroup ztexfmt Z Texture format
 * @{
 */

#define GX_TF_Z8			(0x1|_GX_TF_ZTF)			/*!< For texture copy, specifies upper 8 bits of Z */
#define GX_TF_Z16			(0x3|_GX_TF_ZTF)			/*!< For texture copy, specifies upper 16 bits of Z */
#define GX_TF_Z24X8			(0x6|_GX_TF_ZTF)			/*!< For texture copy, copies 24 Z bits and 0xFF */

/*! @} */

#define GX_CTF_Z4			(0x0|_GX_TF_ZTF|_GX_TF_CTF)			/*!< For copying 4 upper bits from Z */
#define GX_CTF_Z8M			(0x9|_GX_TF_ZTF|_GX_TF_CTF)			/*!< For copying the middle 8 bits of Z */
#define GX_CTF_Z8L			(0xA|_GX_TF_ZTF|_GX_TF_CTF)			/*!< For copying the lower 8 bits of Z */
#define GX_CTF_Z16L			(0xC|_GX_TF_ZTF|_GX_TF_CTF)			/*!< For copying the lower 16 bits of Z */

#define GX_TF_A8			GX_CTF_A8

/*! @} */

/* gx tlut size */
#define GX_TLUT_16			1	// number of 16 entry blocks.
#define GX_TLUT_32			2
#define GX_TLUT_64			4
#define GX_TLUT_128			8
#define GX_TLUT_256			16
#define GX_TLUT_512			32
#define GX_TLUT_1K			64
#define GX_TLUT_2K			128
#define GX_TLUT_4K			256
#define GX_TLUT_8K			512
#define GX_TLUT_16K			1024

/*! \addtogroup ztexop Z Texture operator
 * @{
 */

#define GX_ZT_DISABLE		0
#define GX_ZT_ADD			1			/*!< Add a Z texel to reference Z */
#define GX_ZT_REPLACE		2			/*!< Replace reference Z with Z texel */
#define GX_MAX_ZTEXOP		3

/*! @} */


/*! \addtogroup texgentyp Texture coordinate generation type
 * @{
 */
#define GX_TG_MTX3x4		0			/*!< 3x4 matrix multiply on the input attribute and generate S,T,Q coordinates; S,T are then divided
										* by Q to produce the actual 2D texture coordinates. */
#define GX_TG_MTX2x4		1			/*!< 2x4 matrix multiply on the input attribute and generate S,T texture coordinates. */
#define GX_TG_BUMP0			2			/*!< Use light 0 in the bump map calculation. */
#define GX_TG_BUMP1			3			/*!< Use light 1 in the bump map calculation. */
#define GX_TG_BUMP2			4			/*!< Use light 2 in the bump map calculation. */
#define GX_TG_BUMP3			5			/*!< Use light 3 in the bump map calculation. */
#define GX_TG_BUMP4			6			/*!< Use light 4 in the bump map calculation. */
#define GX_TG_BUMP5			7			/*!< Use light 5 in the bump map calculation. */
#define GX_TG_BUMP6			8			/*!< Use light 6 in the bump map calculation. */
#define GX_TG_BUMP7			9			/*!< Use light 7 in the bump map calculation. */
#define GX_TG_SRTG			10			/*!< Coordinates generated from vertex lighting results; one of the color channel results is converted
										* into texture coordinates. */
/*! @} */

/*! \addtogroup texgensrc Texture coordinate source
 * @{
 */
#define GX_TG_POS			0
#define GX_TG_NRM			1
#define GX_TG_BINRM			2
#define GX_TG_TANGENT		3
#define GX_TG_TEX0			4
#define GX_TG_TEX1			5
#define GX_TG_TEX2			6
#define GX_TG_TEX3			7
#define GX_TG_TEX4			8
#define GX_TG_TEX5			9
#define GX_TG_TEX6			10
#define GX_TG_TEX7			11
#define GX_TG_TEXCOORD0		12
#define GX_TG_TEXCOORD1		13
#define GX_TG_TEXCOORD2		14
#define GX_TG_TEXCOORD3		15
#define GX_TG_TEXCOORD4		16
#define GX_TG_TEXCOORD5		17
#define GX_TG_TEXCOORD6		18
#define GX_TG_COLOR0		19
#define GX_TG_COLOR1		20
/*! @} */

/*! \addtogroup compare Compare type
 * @{
 */
#define GX_NEVER			0
#define GX_LESS				1
#define GX_EQUAL			2
#define GX_LEQUAL			3
#define GX_GREATER			4
#define GX_NEQUAL			5
#define GX_GEQUAL			6
#define GX_ALWAYS			7
/*! @} */

/* Text Wrap Mode */
#define GX_CLAMP			0
#define GX_REPEAT			1
#define GX_MIRROR			2
#define GX_MAXTEXWRAPMODE	3

/*! \addtogroup blendmode Blending type
 * @{
 */
#define GX_BM_NONE			0			/*!< Write input directly to EFB */
#define GX_BM_BLEND			1			/*!< Blend using blending equation */
#define GX_BM_LOGIC			2			/*!< Blend using bitwise operation */
#define GX_BM_SUBTRACT		3			/*!< Input subtracts from existing pixel */
#define GX_MAX_BLENDMODE	4
/*! @} */

/*! \addtogroup blendfactor Blending control
 * \details Each pixel (source or destination) is multiplied by any of these controls.
 * @{
 */
#define GX_BL_ZERO			0			/*!< 0.0 */
#define GX_BL_ONE			1			/*!< 1.0 */
#define GX_BL_SRCCLR		2			/*!< source color */
#define GX_BL_INVSRCCLR		3			/*!< 1.0 - (source color) */
#define GX_BL_SRCALPHA		4			/*!< source alpha */
#define GX_BL_INVSRCALPHA	5			/*!< 1.0 - (source alpha) */
#define GX_BL_DSTALPHA		6			/*!< framebuffer alpha */
#define GX_BL_INVDSTALPHA	7			/*!< 1.0 - (FB alpha) */
#define GX_BL_DSTCLR		GX_BL_SRCCLR
#define GX_BL_INVDSTCLR		GX_BL_INVSRCCLR
/*! @} */

/*! \addtogroup logicop Logical operation type
 * \details Destination (dst) acquires the value of one of these operations, given in C syntax.
 * @{
 */
#define GX_LO_CLEAR			0			/*!< 0 */
#define GX_LO_AND			1			/*!< src & dst */
#define GX_LO_REVAND		2			/*!< src & ~dst */
#define GX_LO_COPY			3			/*!< src */
#define GX_LO_INVAND		4			/*!< ~src & dst */
#define GX_LO_NOOP			5			/*!< dst */
#define GX_LO_XOR			6			/*!< src ^ dst */
#define GX_LO_OR			7			/*!< src | dst */
#define GX_LO_NOR			8			/*!< ~(src | dst) */
#define GX_LO_EQUIV			9			/*!< ~(src ^ dst) */
#define GX_LO_INV			10			/*!< ~dst */
#define GX_LO_REVOR			11			/*!< src | ~dst */
#define GX_LO_INVCOPY		12			/*!< ~src */
#define GX_LO_INVOR			13			/*!< ~src | dst */
#define GX_LO_NAND			14			/*!< ~(src & dst) */
#define GX_LO_SET			15			/*!< 1 */
/*! @} */

/*! \addtogroup texoff Texture offset value
 * \brief Used for texturing points or lines.
 * @{
 */
#define GX_TO_ZERO			0
#define GX_TO_SIXTEENTH		1
#define GX_TO_EIGHTH		2
#define GX_TO_FOURTH		3
#define GX_TO_HALF			4
#define GX_TO_ONE			5
#define GX_MAX_TEXOFFSET	6
/*! @} */

/*! \addtogroup tevdefmode TEV combiner operation
 * \brief Color/Alpha combiner modes for GX_SetTevOp().
 *
 * \details For these equations, <i>Cv</i> is the output color for the stage, <i>Cr</i> is the output color of previous stage, and <i>Ct</i> is the texture color. <i>Av</i> is the output
 * alpha for a stage, <i>Ar</i> is the output alpha of previous stage, and <i>At</i> is the texture alpha. As a special case, rasterized color
 * (<tt>GX_CC_RASC</tt>) is used as <i>Cr</i> and rasterized alpha (<tt>GX_CA_RASA</tt>) is used as <i>Ar</i> at the first TEV stage because there is no previous stage.
 *
 * @{
 */

#define GX_MODULATE			0			/*!< <i>Cv</i>=<i>CrCt</i>; <i>Av</i>=<i>ArAt</i> */
#define GX_DECAL			1			/*!< <i>Cv</i>=(1-<i>At</i>)<i>Cr</i> + <i>AtCt</i>; <i>Av</i>=<i>Ar</i> */
#define GX_BLEND			2			/*!< <i>Cv=(1-<i>Ct</i>)<i>Cr</i> + <i>Ct</i>; <i>Av</i>=<i>AtAr</i> */
#define GX_REPLACE			3			/*!< <i>Cv=<i>Ct</i>; <i>Ar=<i>At</i> */
#define GX_PASSCLR			4			/*!< <i>Cv=<i>Cr</i>; <i>Av=<i>Ar</i> */

/*! @} */


/*! \addtogroup tevcolorarg TEV color combiner input
 * @{
 */

#define GX_CC_CPREV			0				/*!< Use the color value from previous TEV stage */
#define GX_CC_APREV			1				/*!< Use the alpha value from previous TEV stage */
#define GX_CC_C0			2				/*!< Use the color value from the color/output register 0 */
#define GX_CC_A0			3				/*!< Use the alpha value from the color/output register 0 */
#define GX_CC_C1			4				/*!< Use the color value from the color/output register 1 */
#define GX_CC_A1			5				/*!< Use the alpha value from the color/output register 1 */
#define GX_CC_C2			6				/*!< Use the color value from the color/output register 2 */
#define GX_CC_A2			7				/*!< Use the alpha value from the color/output register 2 */
#define GX_CC_TEXC			8				/*!< Use the color value from texture */
#define GX_CC_TEXA			9				/*!< Use the alpha value from texture */
#define GX_CC_RASC			10				/*!< Use the color value from rasterizer */
#define GX_CC_RASA			11				/*!< Use the alpha value from rasterizer */
#define GX_CC_ONE			12
#define GX_CC_HALF			13
#define GX_CC_KONST			14
#define GX_CC_ZERO			15				/*!< Use to pass zero value */

/*! @} */


/*! \addtogroup tevalphaarg TEV alpha combiner input
 * @{
 */

#define GX_CA_APREV			0				/*!< Use the alpha value from previous TEV stage */
#define GX_CA_A0			1				/*!< Use the alpha value from the color/output register 0 */
#define GX_CA_A1			2				/*!< Use the alpha value from the color/output register 1 */
#define GX_CA_A2			3				/*!< Use the alpha value from the color/output register 2 */
#define GX_CA_TEXA			4				/*!< Use the alpha value from texture */
#define GX_CA_RASA			5				/*!< Use the alpha value from rasterizer */
#define GX_CA_KONST			6
#define GX_CA_ZERO			7				/*!< Use to pass zero value */

/*! @} */


/*! \addtogroup tevstage TEV stage
 * \details The GameCube's Graphics Processor (GP) can use up to 16 stages to compute a texel for a particular surface.
 * By default, each texture will use two stages, but it can be configured through various functions calls.
 *
 * \note This is different from \ref texmapid s, where textures are loaded into.
 * @{
 */

#define GX_TEVSTAGE0		0
#define GX_TEVSTAGE1		1
#define GX_TEVSTAGE2		2
#define GX_TEVSTAGE3		3
#define GX_TEVSTAGE4		4
#define GX_TEVSTAGE5		5
#define GX_TEVSTAGE6		6
#define GX_TEVSTAGE7		7
#define GX_TEVSTAGE8		8
#define GX_TEVSTAGE9		9
#define GX_TEVSTAGE10		10
#define GX_TEVSTAGE11		11
#define GX_TEVSTAGE12		12
#define GX_TEVSTAGE13		13
#define GX_TEVSTAGE14		14
#define GX_TEVSTAGE15		15
#define GX_MAX_TEVSTAGE		16

/*! @} */


/*! \addtogroup tevop TEV combiner operator
 * @{
 */

#define GX_TEV_ADD				0
#define GX_TEV_SUB				1
#define GX_TEV_COMP_R8_GT		8
#define GX_TEV_COMP_R8_EQ		9
#define GX_TEV_COMP_GR16_GT		10
#define GX_TEV_COMP_GR16_EQ		11
#define GX_TEV_COMP_BGR24_GT	12
#define GX_TEV_COMP_BGR24_EQ	13
#define GX_TEV_COMP_RGB8_GT		14
#define GX_TEV_COMP_RGB8_EQ		15
#define GX_TEV_COMP_A8_GT		GX_TEV_COMP_RGB8_GT	 // for alpha channel
#define GX_TEV_COMP_A8_EQ		GX_TEV_COMP_RGB8_EQ  // for alpha channel

/*! @} */


/*! \addtogroup tevbias TEV bias value
 * @{
 */

#define GX_TB_ZERO				0
#define GX_TB_ADDHALF			1
#define GX_TB_SUBHALF			2
#define GX_MAX_TEVBIAS			3

/*! @} */


/*! \addtogroup tevclampmode TEV clamping mode
 * \note These modes are used for a function which is not implementable on production (i.e. retail) GameCube hardware.
 * @{
 */

#define GX_TC_LINEAR			0
#define GX_TC_GE				1
#define GX_TC_EQ				2
#define GX_TC_LE				3
#define GX_MAX_TEVCLAMPMODE		4

/*! @} */


/*! \addtogroup tevscale TEV scale value
 * @{
 */

#define GX_CS_SCALE_1			0
#define GX_CS_SCALE_2			1
#define GX_CS_SCALE_4			2
#define GX_CS_DIVIDE_2			3
#define GX_MAX_TEVSCALE			4

/*! @} */


/*! \addtogroup tevcoloutreg TEV color/output register
 * @{
 */

#define GX_TEVPREV				0			/*!< Default register for passing results from one stage to another. */
#define GX_TEVREG0				1
#define GX_TEVREG1				2
#define GX_TEVREG2				3
#define GX_MAX_TEVREG			4

/*! @} */


/*! \addtogroup cullmode Backface culling mode
 * @{
 */
#define GX_CULL_NONE			0			/*!< Do not cull any primitives. */
#define GX_CULL_FRONT			1			/*!< Cull front-facing primitives. */
#define GX_CULL_BACK			2			/*!< Cull back-facing primitives. */
#define GX_CULL_ALL				3			/*!< Cull all primitives. */
/*! @} */

/*! \addtogroup texmapid texture map slot
 * \brief Texture map slots to hold textures in.
 *
 * \details The GameCube's Graphics Processor (GP) can apply up to eight textures to a single surface. Those textures
 * are assigned one of these slots. Various operations used on or with a particular texture will also take one of these
 * items, including operations regarding texture coordinate generation (although not necessarily on the same slot).
 *
 * \note This is different from \ref tevstage s, which are the actual quanta for work with textures.
 * @{
 */
#define GX_TEXMAP0				0			/*!< Texture map slot 0 */
#define GX_TEXMAP1				1			/*!< Texture map slot 1 */
#define GX_TEXMAP2				2			/*!< Texture map slot 2 */
#define GX_TEXMAP3				3			/*!< Texture map slot 3 */
#define GX_TEXMAP4				4			/*!< Texture map slot 4 */
#define GX_TEXMAP5				5			/*!< Texture map slot 5 */
#define GX_TEXMAP6				6			/*!< Texture map slot 6 */
#define GX_TEXMAP7				7			/*!< Texture map slot 7 */
#define GX_MAX_TEXMAP			8
#define GX_TEXMAP_NULL			0xff			/*!< No texmap */
#define GX_TEXMAP_DISABLE		0x100			/*!< Disable texmap lookup for this texmap slot (use bitwise OR with a texture map slot). */
/*! @} */

/*! \addtogroup alphaop Alpha combine control
 * @{
 */
#define GX_AOP_AND				0
#define GX_AOP_OR				1
#define GX_AOP_XOR				2
#define GX_AOP_XNOR				3
#define GX_MAX_ALPHAOP			4
/*! @} */

/*! \addtogroup tevkcolorid TEV constant color register
 * @{
 */
#define GX_KCOLOR0				0			/*!< Constant register 0 */
#define GX_KCOLOR1				1			/*!< Constant register 1 */
#define GX_KCOLOR2				2			/*!< Constant register 2 */
#define GX_KCOLOR3				3			/*!< Constant register 3 */
#define GX_KCOLOR_MAX			4
/*! @} */

/*! \addtogroup tevkcolorsel TEV constant color selection
 * @{
 */
#define GX_TEV_KCSEL_1					0x00			/*!< constant 1.0 */
#define GX_TEV_KCSEL_7_8				0x01			/*!< constant 7/8 */
#define GX_TEV_KCSEL_3_4				0x02			/*!< constant 3/4 */
#define GX_TEV_KCSEL_5_8				0x03			/*!< constant 5/8 */
#define GX_TEV_KCSEL_1_2				0x04			/*!< constant 1/2 */
#define GX_TEV_KCSEL_3_8				0x05			/*!< constant 3/8 */
#define GX_TEV_KCSEL_1_4				0x06			/*!< constant 1/4 */
#define GX_TEV_KCSEL_1_8				0x07			/*!< constant 1/8 */
#define GX_TEV_KCSEL_K0					0x0C			/*!< K0[RGB] register */
#define GX_TEV_KCSEL_K1					0x0D			/*!< K1[RGB] register */
#define GX_TEV_KCSEL_K2					0x0E			/*!< K2[RGB] register */
#define GX_TEV_KCSEL_K3					0x0F			/*!< K3[RGB] register */
#define GX_TEV_KCSEL_K0_R				0x10			/*!< K0[RRR] register */
#define GX_TEV_KCSEL_K1_R				0x11			/*!< K1[RRR] register */
#define GX_TEV_KCSEL_K2_R				0x12			/*!< K2[RRR] register */
#define GX_TEV_KCSEL_K3_R				0x13			/*!< K3[RRR] register */
#define GX_TEV_KCSEL_K0_G				0x14			/*!< K0[GGG] register */
#define GX_TEV_KCSEL_K1_G				0x15			/*!< K1[GGG] register */
#define GX_TEV_KCSEL_K2_G				0x16			/*!< K2[GGG] register */
#define GX_TEV_KCSEL_K3_G				0x17			/*!< K3[GGG] register */
#define GX_TEV_KCSEL_K0_B				0x18			/*!< K0[BBB] register */
#define GX_TEV_KCSEL_K1_B				0x19			/*!< K1[BBB] register */
#define GX_TEV_KCSEL_K2_B				0x1A			/*!< K2[BBB] register */
#define GX_TEV_KCSEL_K3_B				0x1B			/*!< K3[RBB] register */
#define GX_TEV_KCSEL_K0_A				0x1C			/*!< K0[AAA] register */
#define GX_TEV_KCSEL_K1_A				0x1D			/*!< K1[AAA] register */
#define GX_TEV_KCSEL_K2_A				0x1E			/*!< K2[AAA] register */
#define GX_TEV_KCSEL_K3_A				0x1F			/*!< K3[AAA] register */
/*! @} */

/*! \addtogroup tevkalphasel TEV constant alpha selection
 * @{
 */
#define GX_TEV_KASEL_1					0x00			/*!< constant 1.0 */
#define GX_TEV_KASEL_7_8				0x01			/*!< constant 7/8 */
#define GX_TEV_KASEL_3_4				0x02			/*!< constant 3/4 */
#define GX_TEV_KASEL_5_8				0x03			/*!< constant 5/8 */
#define GX_TEV_KASEL_1_2				0x04			/*!< constant 1/2 */
#define GX_TEV_KASEL_3_8				0x05			/*!< constant 3/8 */
#define GX_TEV_KASEL_1_4				0x06			/*!< constant 1/4 */
#define GX_TEV_KASEL_1_8				0x07			/*!< constant 1/8 */
#define GX_TEV_KASEL_K0_R				0x10			/*!< K0[R] register */
#define GX_TEV_KASEL_K1_R				0x11			/*!< K1[R] register */
#define GX_TEV_KASEL_K2_R				0x12			/*!< K2[R] register */
#define GX_TEV_KASEL_K3_R				0x13			/*!< K3[R] register */
#define GX_TEV_KASEL_K0_G				0x14			/*!< K0[G] register */
#define GX_TEV_KASEL_K1_G				0x15			/*!< K1[G] register */
#define GX_TEV_KASEL_K2_G				0x16			/*!< K2[G] register */
#define GX_TEV_KASEL_K3_G				0x17			/*!< K3[G] register */
#define GX_TEV_KASEL_K0_B				0x18			/*!< K0[B] register */
#define GX_TEV_KASEL_K1_B				0x19			/*!< K1[B] register */
#define GX_TEV_KASEL_K2_B				0x1A			/*!< K2[B] register */
#define GX_TEV_KASEL_K3_B				0x1B			/*!< K3[B] register */
#define GX_TEV_KASEL_K0_A				0x1C			/*!< K0[A] register */
#define GX_TEV_KASEL_K1_A				0x1D			/*!< K1[A] register */
#define GX_TEV_KASEL_K2_A				0x1E			/*!< K2[A] register */
#define GX_TEV_KASEL_K3_A				0x1F			/*!< K3[A] register */
/*! @} */


/*! \addtogroup tevswapsel TEV color swap table entry
 * @{
 */

#define GX_TEV_SWAP0					0
#define GX_TEV_SWAP1					1
#define GX_TEV_SWAP2					2
#define GX_TEV_SWAP3					3
#define GX_MAX_TEVSWAP					4

/*! @} */


/* tev color chan */
#define GX_CH_RED						0
#define GX_CH_GREEN						1
#define GX_CH_BLUE						2
#define GX_CH_ALPHA						3

/*! \addtogroup indtexstage Indirect texture stage
 * @{
 */
#define GX_INDTEXSTAGE0					0
#define GX_INDTEXSTAGE1					1
#define GX_INDTEXSTAGE2					2
#define GX_INDTEXSTAGE3					3
#define GX_MAX_INDTEXSTAGE				4
/*! @} */

/*! \addtogroup indtexformat Indirect texture format
 * \details Bits for the indirect offsets are extracted from the high end of each component byte. Bits for the bump alpha
 * are extraced off the low end of the byte. For <tt>GX_ITF_8</tt>, the byte is duplicated for the offset and the bump alpha.
 * @{
 */
#define GX_ITF_8						0
#define GX_ITF_5						1
#define GX_ITF_4						2
#define GX_ITF_3						3
#define GX_MAX_ITFORMAT					4
/*! @} */

/*! \addtogroup indtexbias Indirect texture bias select
 * \brief Indicates which components of the indirect offset should receive a bias value.
 *
 * \details The bias is fixed at -128 for <tt>GX_ITF_8</tt> and +1 for the other formats. The bias happens prior to the indirect matrix multiply.
 * @{
 */
#define GX_ITB_NONE						0
#define GX_ITB_S						1
#define GX_ITB_T						2
#define GX_ITB_ST						3
#define GX_ITB_U						4
#define GX_ITB_SU						5
#define GX_ITB_TU						6
#define GX_ITB_STU						7
#define GX_MAX_ITBIAS					8
/*! @} */

/*! \addtogroup indtexmtx Indirect texture matrix
 * @{
 */
#define GX_ITM_OFF						0			/*!< Specifies a matrix of all zeroes. */
#define GX_ITM_0						1			/*!< Specifies indirect matrix 0, indirect scale 0. */
#define GX_ITM_1						2			/*!< Specifies indirect matrix 1, indirect scale 1. */
#define GX_ITM_2						3			/*!< Specifies indirect matrix 2, indirect scale 2. */
#define GX_ITM_S0						5			/*!< Specifies dynamic S-type matrix, indirect scale 0. */
#define GX_ITM_S1						6			/*!< Specifies dynamic S-type matrix, indirect scale 1. */
#define GX_ITM_S2						7			/*!< Specifies dynamic S-type matrix, indirect scale 2. */
#define GX_ITM_T0						9			/*!< Specifies dynamic T-type matrix, indirect scale 0. */
#define GX_ITM_T1						10			/*!< Specifies dynamic T-type matrix, indirect scale 1. */
#define GX_ITM_T2						11			/*!< Specifies dynamic T-type matrix, indirect scale 2. */
/*! @} */

/*! \addtogroup indtexwrap Indirect texture wrap value
 * \brief Indicates whether the regular texture coordinate should be wrapped before being added to the offset.
 *
 * \details <tt>GX_ITW_OFF</tt> specifies no wrapping. <tt>GX_ITW_0</tt> will zero out the regular texture coordinate.
 * @{
 */
#define GX_ITW_OFF						0
#define GX_ITW_256						1
#define GX_ITW_128						2
#define GX_ITW_64						3
#define GX_ITW_32						4
#define GX_ITW_16						5
#define GX_ITW_0						6
#define GX_MAX_ITWRAP					7
/*! @} */

/*! \addtogroup indtexalphasel Indirect texture bump alpha select
 * \brief Indicates which offset component should provide the "bump" alpha output for the given TEV stage.
 *
 * \note Bump alpha is not available for TEV stage 0.
 * @{
 */
#define GX_ITBA_OFF						0
#define GX_ITBA_S						1
#define GX_ITBA_T						2
#define GX_ITBA_U						3
#define GX_MAX_ITBALPHA					4
/*! @} */

/*! \addtogroup indtexscale Indirect texture scale
 * \brief Specifies an additional scale value that may be applied to the texcoord used for an indirect initial lookup (not a TEV stage regular lookup).
 *
 * \details The scale value is a fraction; thus <tt>GX_ITS_32</tt> means to divide the texture coordinate values by 32.
 * @{
 */
#define GX_ITS_1						0
#define GX_ITS_2						1
#define GX_ITS_4						2
#define GX_ITS_8						3
#define GX_ITS_16						4
#define GX_ITS_32						5
#define GX_ITS_64						6
#define GX_ITS_128						7
#define GX_ITS_256						8
#define GX_MAX_ITSCALE					9
/*! @} */

/*! \addtogroup fogtype Fog equation control
 * @{
 */
#define GX_FOG_NONE						0

#define GX_FOG_PERSP_LIN				2
#define GX_FOG_PERSP_EXP				4
#define GX_FOG_PERSP_EXP2				5
#define GX_FOG_PERSP_REVEXP				6
#define GX_FOG_PERSP_REVEXP2			7

#define GX_FOG_ORTHO_LIN				10
#define GX_FOG_ORTHO_EXP				12
#define GX_FOG_ORTHO_EXP2				13
#define GX_FOG_ORTHO_REVEXP				14
#define GX_FOG_ORTHO_REVEXP2			15

#define GX_FOG_LIN						GX_FOG_PERSP_LIN
#define GX_FOG_EXP						GX_FOG_PERSP_EXP
#define GX_FOG_EXP2						GX_FOG_PERSP_EXP2
#define GX_FOG_REVEXP  					GX_FOG_PERSP_REVEXP
#define GX_FOG_REVEXP2 					GX_FOG_PERSP_REVEXP2
/*! @} */


/* pixel format */
#define GX_PF_RGB8_Z24					0
#define GX_PF_RGBA6_Z24					1
#define GX_PF_RGB565_Z16				2
#define GX_PF_Z24						3
#define GX_PF_Y8						4
#define GX_PF_U8						5
#define GX_PF_V8						6
#define GX_PF_YUV420					7

/*! \addtogroup zfmt Compressed Z format
 * @{
 */
#define GX_ZC_LINEAR					0
#define GX_ZC_NEAR						1
#define GX_ZC_MID						2
#define GX_ZC_FAR						3
/*! @} */

/*! \addtogroup xfbclamp XFB clamp modes
 * @{
 */

#define GX_CLAMP_NONE					0
#define GX_CLAMP_TOP					1
#define GX_CLAMP_BOTTOM					2

/*! @} */


/*! \addtogroup gammamode Gamma values
 * @{
 */

#define GX_GM_1_0						0
#define GX_GM_1_7						1
#define GX_GM_2_2						2

/*! @} */


/*! \addtogroup copymode EFB copy mode
 * \brief Controls whether all lines, only even lines, or only odd lines are copied from the EFB.
 * @{
 */
#define GX_COPY_PROGRESSIVE				0
#define GX_COPY_INTLC_EVEN				2
#define GX_COPY_INTLC_ODD				3
/*! @} */

/*! \addtogroup alphareadmode Alpha read mode
 * @{
 */
#define GX_READ_00						0			/*!< Always read 0x00. */
#define GX_READ_FF						1			/*!< Always read 0xFF. */
#define GX_READ_NONE					2			/*!< Always read the real alpha value. */
/*! @} */

/*! \addtogroup texcachesize Texture cache size
 * \brief Size of texture cache regions.
 * @{
 */
#define GX_TEXCACHE_32K					0
#define GX_TEXCACHE_128K				1
#define GX_TEXCACHE_512K				2
#define GX_TEXCACHE_NONE				3
/*! @} */

/*! \addtogroup distattnfn Brightness decreasing function
 * \brief Type of the brightness decreasing function by distance.
 * @{
 */
#define GX_DA_OFF						0
#define GX_DA_GENTLE					1
#define GX_DA_MEDIUM					2
#define GX_DA_STEEP						3
/*! @} */

/*! \addtogroup spotfn Spot illumination distribution function
 * @{
 */
#define GX_SP_OFF						0
#define GX_SP_FLAT						1
#define GX_SP_COS						2
#define GX_SP_COS2						3
#define GX_SP_SHARP						4
#define GX_SP_RING1						5
#define GX_SP_RING2						6
/*! @} */

/*! \addtogroup texfilter Texture filter types
 * @{
 */
#define GX_NEAR							0			/*!< Point sampling, no mipmap */
#define GX_LINEAR						1			/*!< Bilinear filtering, no mipmap */
#define GX_NEAR_MIP_NEAR				2			/*!< Point sampling, discrete mipmap */
#define GX_LIN_MIP_NEAR					3			/*!< Bilinear filtering, discrete mipmap */
#define GX_NEAR_MIP_LIN					4			/*!< Point sampling, linear mipmap */
#define GX_LIN_MIP_LIN					5			/*!< Trilinear filtering */
/*! @} */

/*! \addtogroup anisotropy Maximum anisotropy filter control
 * @{
 */
#define GX_ANISO_1						0
#define GX_ANISO_2						1
#define GX_ANISO_4						2
#define GX_MAX_ANISOTROPY				3
/*! @} */

/*! \addtogroup vcachemetrics Vertex cache performance counter
 * @{
 */
#define GX_VC_POS						0
#define GX_VC_NRM						1
#define GX_VC_CLR0						2
#define GX_VC_CLR1						3
#define GX_VC_TEX0						4
#define GX_VC_TEX1						5
#define GX_VC_TEX2						6
#define GX_VC_TEX3						7
#define GX_VC_TEX4						8
#define GX_VC_TEX5						9
#define GX_VC_TEX6						10
#define GX_VC_TEX7						11
#define GX_VC_ALL						15
/*! @} */

/*! \addtogroup perf0metrics Performance counter 0 metric
 * \details Performance counter 0 is used to measure attributes dealing with geometry and primitives, such as triangle counts and clipping ratios.
 *
 * \note <tt>GX_PERF0_XF_*</tt> measure how many GP cycles are spent in each stage of the XF.<br><br>
 *
 * \note The triangle metrics (<tt>GX_PERF0_TRIANGLES_*</tt>) allow counting triangles under specific conditions or with specific attributes.<br><br>
 *
 * \note <tt>GX_PERF0_TRIANGLES_*TEX</tt> count triangles based on the number of texture coordinates supplied; <tt>GX_PERF0_TRIANGLES_*CLR</tt> count
 * triangles based on the number of colors supplied.<br><br>
 *
 * \note The quad metrics allow you to count the number of quads (2x2 pixels) the GP processes. The term <i>coverage</i> is used to indicate how many
 * pixels in the quad are actually part of the triangle being rasterized. For example, a coverage of 4 means all pixels in the quad intersect the
 * triangle. A coverage of 1 indicates that only 1 pixel in the quad intersected the triangle.
 * @{
 */
#define GX_PERF0_VERTICES				0			/*!< Number of vertices processed by the GP. */
#define GX_PERF0_CLIP_VTX				1			/*!< Number of vertices that were clipped by the GP. */
#define GX_PERF0_CLIP_CLKS				2			/*!< Number of GP clocks spent clipping. */
#define GX_PERF0_XF_WAIT_IN				3			/*!< Number of cycles the XF is waiting on input. If the XF is waiting a large percentage
													* of the total time, it may indicate that the CPU is not supplying data fast enough to
													* keep the GP busy. */
#define GX_PERF0_XF_WAIT_OUT			4			/*!< Number of cycles the XF waits to send its output to the rest of the GP pipeline. If
													* the XF cannot output, it may indicate that the GP is currently fill-rate limited. */
#define GX_PERF0_XF_XFRM_CLKS			5			/*!< Number of cycles the transform engine is busy. */
#define GX_PERF0_XF_LIT_CLKS			6			/*!< Number of cycles the lighting engine is busy. */
#define GX_PERF0_XF_BOT_CLKS			7			/*!< Number of cycles the bottom of the pipe (result combiner) is busy. */
#define GX_PERF0_XF_REGLD_CLKS			8			/*!< Number of cycles are spent loading XF state registers. */
#define GX_PERF0_XF_REGRD_CLKS			9			/*!< Number of cycles the XF reads the state registers. */
#define GX_PERF0_CLIP_RATIO				10
#define GX_PERF0_TRIANGLES				11			/*!< Number of triangles. */
#define GX_PERF0_TRIANGLES_CULLED		12			/*!< Number of triangles that <i>failed</i> the front-face/back-face culling test. */
#define GX_PERF0_TRIANGLES_PASSED		13			/*!< Number of triangles that <i>passed</i> the front-face/back-face culling test. */
#define GX_PERF0_TRIANGLES_SCISSORED	14			/*!< Number of triangles that are scissored. */
#define GX_PERF0_TRIANGLES_0TEX			15
#define GX_PERF0_TRIANGLES_1TEX			16
#define GX_PERF0_TRIANGLES_2TEX			17
#define GX_PERF0_TRIANGLES_3TEX			18
#define GX_PERF0_TRIANGLES_4TEX			19
#define GX_PERF0_TRIANGLES_5TEX			20
#define GX_PERF0_TRIANGLES_6TEX			21
#define GX_PERF0_TRIANGLES_7TEX			22
#define GX_PERF0_TRIANGLES_8TEX			23
#define GX_PERF0_TRIANGLES_0CLR			24
#define GX_PERF0_TRIANGLES_1CLR			25
#define GX_PERF0_TRIANGLES_2CLR			26
#define GX_PERF0_QUAD_0CVG				27			/*!< Number of quads having zero coverage. */
#define GX_PERF0_QUAD_NON0CVG			28			/*!< Number of quads having coverage greater than zero. */
#define GX_PERF0_QUAD_1CVG				29			/*!< Number of quads with 1 pixel coverage. */
#define GX_PERF0_QUAD_2CVG				30			/*!< Number of quads with 2 pixel coverage. */
#define GX_PERF0_QUAD_3CVG				31			/*!< Number of quads with 3 pixel coverage. */
#define GX_PERF0_QUAD_4CVG				32			/*!< Number of quads with 4 pixel coverage. */
#define GX_PERF0_AVG_QUAD_CNT			33			/*!< Average quad count; average based on what is unknown */
#define GX_PERF0_CLOCKS					34			/*!< Number of GP clocks that have elapsed since the previous call to GX_ReadGP0Metric(). */
#define GX_PERF0_NONE					35			/*!< Disables performance measurement for perf0 and resets the counter. */
/*! @} */

/*! \addtogroup perf1metrics Performance counter 1 metric
 * \details Performance counter 1 is used for measuring texturing and caching performance as well as FIFO performance.
 *
 * \note <tt>GX_PERF1_TC_*</tt> can be used to compute the texture cache (TC) miss rate. The <tt>TC_CHECK*</tt> parameters count how many texture cache lines are
 * accessed for each pixel. In the worst case, for a mipmap, up to 8 cache lines may be accessed to produce one textured pixel.
 * <tt>GX_PERF1_TC_MISS</tt> counts how many of those accesses missed the texture cache. To compute the miss rate, divide <tt>GX_PERF1_TC_MISS</tt> by the sum of all four
 * <tt>GX_PERF1_TC_CHECK*</tt> values.<br><br>
 *
 * \note <tt>GX_PERF1_VC_*</tt> count different vertex cache stall conditions.
 * @{
 */
#define GX_PERF1_TEXELS					0			/*!< Number of texels processed by the GP. */
#define GX_PERF1_TX_IDLE				1			/*!< Number of clocks that the texture unit (TX) is idle. */
#define GX_PERF1_TX_REGS				2			/*!< Number of GP clocks spent writing to state registers in the TX unit. */
#define GX_PERF1_TX_MEMSTALL			3			/*!< Number of GP clocks the TX unit is stalled waiting for main memory. */
#define GX_PERF1_TC_CHECK1_2			4
#define GX_PERF1_TC_CHECK3_4			5
#define GX_PERF1_TC_CHECK5_6			6
#define GX_PERF1_TC_CHECK7_8			7
#define GX_PERF1_TC_MISS				8			/*!< Number of texture cache misses in total? */
#define GX_PERF1_VC_ELEMQ_FULL			9
#define GX_PERF1_VC_MISSQ_FULL			10
#define GX_PERF1_VC_MEMREQ_FULL			11
#define GX_PERF1_VC_STATUS7				12
#define GX_PERF1_VC_MISSREP_FULL		13
#define GX_PERF1_VC_STREAMBUF_LOW		14
#define GX_PERF1_VC_ALL_STALLS			15
#define GX_PERF1_VERTICES				16			/*!< Number of vertices processed by the GP. */
#define GX_PERF1_FIFO_REQ				17			/*!< Number of lines (32B) read from the GP FIFO. */
#define GX_PERF1_CALL_REQ				18			/*!< Number of lines (32B) read from called display lists. */
#define GX_PERF1_VC_MISS_REQ			19			/*!< Number vertex cache miss request. Each miss requests a 32B transfer from main memory. */
#define GX_PERF1_CP_ALL_REQ				20			/*!< Counts all requests (32B/request) from the GP Command Processor (CP). It should be equal to
													* the sum of counts returned by <tt>GX_PERF1_FIFO_REQ</tt>, <tt>GX_PERF1_CALL_REQ</tt>, and <tt>GX_PERF1_VC_MISS_REQ</tt>. */
#define GX_PERF1_CLOCKS					21			/*!< Number of GP clocks that have elapsed since the last call to GX_ReadGP1Metric(). */
#define GX_PERF1_NONE					22			/*!< Disables performance measurement for perf1 and resets the counter. */
/*! @} */

/*! \addtogroup tlutname TLUT name
 * \brief Name of Texture Look-Up Table (TLUT) in texture memory.
 *
 * \details Each table <tt>GX_TLUT0</tt>-<tt>GX_TLUT15</tt> contains 256 entries,16b per entry. <tt>GX_BIGTLUT0</tt>-<tt>3</tt>
 * contains 1024 entries, 16b per entry. Used for configuring texture memory in GX_Init().
 * @{
 */
#define GX_TLUT0						 0
#define GX_TLUT1						 1
#define GX_TLUT2						 2
#define GX_TLUT3						 3
#define GX_TLUT4						 4
#define GX_TLUT5						 5
#define GX_TLUT6						 6
#define GX_TLUT7						 7
#define GX_TLUT8						 8
#define GX_TLUT9						 9
#define GX_TLUT10						10
#define GX_TLUT11						11
#define GX_TLUT12						12
#define GX_TLUT13						13
#define GX_TLUT14						14
#define GX_TLUT15						15
#define GX_BIGTLUT0						16
#define GX_BIGTLUT1						17
#define GX_BIGTLUT2						18
#define GX_BIGTLUT3						19
/*! @} */

#define GX_MAX_VTXDESC					GX_VA_MAXATTR
#define GX_MAX_VTXDESC_LISTSIZE			(GX_VA_MAXATTR+1)

#define GX_MAX_VTXATTRFMT				GX_VA_MAXATTR
#define GX_MAX_VTXATTRFMT_LISTSIZE		(GX_VA_MAXATTR+1)

#define GX_MAX_Z24						0x00ffffff
