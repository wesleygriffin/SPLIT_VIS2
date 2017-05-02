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
        /*_color.rgb = gl_Color;*/
        vec3 normal, lightDir;

        vec4 diffuse, ambient, globalAmbient, specular;

        float NdotL;


        normal = normalize(gl_NormalMatrix * gl_Normal);
        lightDir = normalize(vec3(gl_LightSource[0].position));

/*TWO SIDED LIGHT*/
       /* if(dot(normal, lightDir)<0)normal = - normal;
*/
        NdotL = max(dot(normal, lightDir), 0.0);

        if(NdotL > 0.0)
        {
		// normalize the half-vector, and then compute the
		// cosine (dot product) with the normal
		float NdotHV = max(dot(normal, gl_LightSource[0].halfVector.xyz),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
				pow(NdotHV,gl_FrontMaterial.shininess);
        }
        else
        {
                specular = vec4(0,0,0,0);
        }

        diffuse = gl_Color * gl_LightSource[0].diffuse;
/*gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;*/
        /* Compute the ambient and globalAmbient terms */

        ambient = gl_Color * gl_LightSource[0].ambient;

/*gl_FrontMaterial.ambient * gl_LightSource[0].ambient;*/

        globalAmbient = gl_Color *gl_LightModel.ambient ;
/*gl_LightModel.ambient * gl_FrontMaterial.ambient;*/

        _color =  NdotL * diffuse + globalAmbient +  ambient+specular;

/*-----------------------------------------------------*/
        lightDir = normalize(vec3(gl_LightSource[1].position));
        NdotL = max(dot(normal, lightDir), 0.0);
        if(NdotL > 0.0)
        {
                // normalize the half-vector, and then compute the
                // cosine (dot product) with the normal
                float NdotHV = max(dot(normal, gl_LightSource[1].halfVector.xyz),0.0);
                specular = gl_FrontMaterial.specular * gl_LightSource[1].specular *
                                pow(NdotHV,gl_FrontMaterial.shininess);
        }
        else
        {
                specular = vec4(0,0,0,0);
        }

        diffuse = gl_Color * gl_LightSource[1].diffuse;
/*gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;*/
        ambient = gl_Color * gl_LightSource[1].ambient;
/*gl_FrontMaterial.ambient * gl_LightSource[1].ambient;*/
        _color =  _color+NdotL * diffuse   + ambient + specular;
/*--------------------------------------------------------*/

	gl_Position = ftransform();
	gl_TexCoord[0].xyz = ShadeVertex();
}
