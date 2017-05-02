//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

vec3 ShadeVertex();

varying out vec4 _color;

void main(void)
{
        _color.rgb = gl_Color;
        vec3 normal, lightDir;

        vec4 diffuse, ambient, globalAmbient;

        float NdotL;


        normal = normalize(gl_NormalMatrix * gl_Normal);
        lightDir = normalize(vec3(gl_LightSource[0].position));

/*TWO SIDED LIGHT*/
       /* if(dot(normal, lightDir)<0)normal = - normal;
*/
        NdotL = max(dot(normal, lightDir), 0.0);

        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
        /* Compute the ambient and globalAmbient terms */

        ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

        globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

        _color =  NdotL * diffuse + globalAmbient + ambient;

	gl_Position = ftransform();
	gl_TexCoord[0].xyz = ShadeVertex();
}
