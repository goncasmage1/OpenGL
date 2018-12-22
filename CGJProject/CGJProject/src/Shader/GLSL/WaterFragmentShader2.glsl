#version 330 core

in vec3 Refract;
in vec3 Reflect;
in float Ratio;

out vec4 out_color;
uniform samplerCube skybox;

float fresnel(const vec3 I, const vec3 N, const float ior) 
{ 

    float cosi = clamp(-1, 1, dot(I, N)); 
    float etai = 1, etat = ior; 
    if (cosi > 0) 
	{ 
		float aux = etai;
		etai = etat;
		etat = aux;
	} 
    // Compute sini using Snell's law
    float sint = etai / etat * sqrt(max(0, 1 - cosi * cosi)); 
    // Total internal reflection
    if (sint >= 1) { 
        return 1; 
    } 
    else { 
        float cost = sqrt(max(0.f, 1 - sint * sint)); 
        cosi = abs(cosi); 
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost)); 
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost)); 
        return (Rs * Rs + Rp * Rp) / 2; 
    } 
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
} 

vec3 refraction(vec3 I, vec3 N, float ior)
{
	float cosi = clamp(-1, 1, dot(I, N)); 
    float etai = 1, etat = ior; 
    vec3 n = N; 
    if (cosi < 0) 
	{ 
		cosi = -cosi; 
	} else { 
		float aux = etai;
		etai = etat;
		etat = aux;
		n= -N; 
	} 
    float eta = etai / etat; 
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrt(k)) * n; 
}


const float refractive_water = 1.33;
const float refractive_ice = 1.309;
const float refractive_glass = 1.52;
const float refractive_diamond = 2.42;

void main()
{

	//float Eta = 1.0/ refractive_water;	

	//float Ratio = fresnel(-I, Normal, refractive_water);
	
	vec3 refractColor = vec3 (texture(skybox, Refract));
    vec3 reflectColor = vec3 (texture(skybox, Reflect));
	
	//float Ratio = dot(I, Normal);
	//Ratio = pow(Ratio, 10);
    vec3 color = mix(reflectColor, refractColor, 0.9);
 
    out_color = vec4(color, 1.0);
}

