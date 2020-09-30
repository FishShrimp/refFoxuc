//
// Glow Effect
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Note: This effect file works with EffectEdit.
//

string XFile = "tiger\\tiger.x";   // model
string BIMG  = "misc\\lake.bmp";  // Background image
int    BCLR  = 0xff202080;  // background

// texture
texture Tex0 < string name = "tiger\\tiger.bmp"; >;

// transforms
float4x3 WorldView  : WORLDVIEW; 
float4x4 Projection : PROJECTION;

// light direction (view space)
float3 LightDir < string UIDirectional = "Light Direction"; > = normalize(float3(0.0f, 0.0f, 1.0f));

// glow parameters
float4 GlowColor     = float4(0.5f, 0.2f, 0.2f, 1.0f);
float4 GlowAmbient   = float4(0.2f, 0.2f, 0.0f, 0.0f);
float  GlowThickness = 0.015f;

struct VSTEXTURE_OUTPUT
{
    float4 Position : POSITION;
    float4 Diffuse  : COLOR;
    float2 TexCoord : TEXCOORD0;
};

// draws unskinned object with one texture and one directional light.
VSTEXTURE_OUTPUT VSTexture
    (
    float4 Position : POSITION, 
    float3 Normal   : NORMAL,
    float2 TexCoord : TEXCOORD0
    )
{
    VSTEXTURE_OUTPUT Out = (VSTEXTURE_OUTPUT)0;
  
    float3 L = -LightDir;                                   // light direction (view space)
    float3 P = mul(Position, WorldView);                    // position (view space)
    float3 N = normalize(mul(Normal, (float3x3)WorldView)); // normal (view space)
   
    Out.Position = mul(float4(P, 1), Projection);   // projected position
    Out.Diffuse  = max(0, dot(N, L));               // diffuse 
    Out.TexCoord = TexCoord;                        // texture coordinates
    
    return Out;    
}

struct VSGLOW_OUTPUT
{
    float4 Position : POSITION;
    float4 Diffuse  : COLOR;
};

// draws a transparent hull of the unskinned object.
VSGLOW_OUTPUT VSGlow
    (
    float4 Position : POSITION, 
    float3 Normal   : NORMAL
    )
{
    VSGLOW_OUTPUT Out = (VSGLOW_OUTPUT)0;

    float3 N = normalize(mul(Normal, (float3x3)WorldView));     // normal (view space)
    float3 P = mul(Position, WorldView) + GlowThickness * N;    // displaced position (view space)
    float3 A = float3(0, 0, 1);                                 // glow axis

    float Power;

    Power  = dot(N, A);
    Power *= Power;
    Power -= 1;
    Power *= Power;     // Power = (1 - (N.A)^2)^2 [ = ((N.A)^2 - 1)^2 ]
    
    Out.Position = mul(float4(P, 1), Projection);   // projected position
    Out.Diffuse  = GlowColor * Power + GlowAmbient; // modulated glow color + glow ambient
    
    return Out;    
}



technique TGlowAndTexture
{
    pass PTexture
    {   
        // single texture/one directional light shader
        VertexShader = compile vs_1_1 VSTexture();
        PixelShader  = NULL;
        
        // texture
        Texture[0] = (Tex0);

        // sampler states
        MinFilter[0] = LINEAR;
        MagFilter[0] = LINEAR;
        MipFilter[0] = POINT;

        // set up texture stage states for single texture modulated by diffuse 
        ColorOp[0]   = MODULATE;
        ColorArg1[0] = TEXTURE;
        ColorArg2[0] = CURRENT;
        AlphaOp[0]   = DISABLE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;
        
    }

    pass PGlow
    {   
        // glow shader
        VertexShader = compile vs_1_1 VSGlow();
        PixelShader  = NULL;
        
        // no texture
        Texture[0] = NULL;

        // enable alpha blending
        AlphaBlendEnable = TRUE;
        SrcBlend         = ONE;
        DestBlend        = ONE;

        // set up texture stage states to use the diffuse color
        ColorOp[0]   = SELECTARG2;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = SELECTARG2;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;
    }
}

technique TGlowOnly
{
    pass PGlow
    {   
        // glow shader
        VertexShader = compile vs_1_1 VSGlow();
        PixelShader  = NULL;
        
        // no texture
        Texture[0] = NULL;

        // enable alpha blending
        AlphaBlendEnable = TRUE;
        SrcBlend         = ONE;
        DestBlend        = ONE;

        // set up texture stage states to use the diffuse color
        ColorOp[0]   = SELECTARG2;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = SELECTARG2;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;
   }
}
