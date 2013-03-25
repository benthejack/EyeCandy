#ifdef GL_ES
precision highp float;
#endif

uniform vec3 lightPos;


uniform vec2 resolution;
uniform vec4 mouse;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{
	
	float sze = resolution.y/3.0;
    float d = distance(gl_FragCoord.xy, resolution/2.0);
	
	vec2 center = resolution/2.0;
	vec4 col;	
	vec2 toCenter = center - gl_FragCoord.xy;
	vec3 lightPos = vec3(cos(time/3.0), sin(time/3.0), cos(time/2.0));
	
	
	if(d < sze){
		col = vec4(0.5,0.5,0.5,0.5);//texture2D(tex0, gl_FragCoord.xy/resolution);
		float sRad  = sqrt(pow(sze, 2.0) - pow(center.y-gl_FragCoord.y,2.0));
		float zComp = sqrt(pow(sRad, 2.0) - pow(center.x-gl_FragCoord.x,2.0));
		float zAxis = d/sze;
		vec3 surfaceNorm = vec3(center.x-gl_FragCoord.x, center.y-gl_FragCoord.y, zComp);
		surfaceNorm = normalize(surfaceNorm);
		
		col *= (dot(normalize(lightPos), normalize(surfaceNorm)));
	}else{
    
		col = vec4(0);
	}
	
	
	
	
	gl_FragColor = col;

}