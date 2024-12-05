#version 330 core

in vec2 chTex; 
in vec2 fragPos; 
out vec4 outCol; 

uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform sampler2D uTex3;
uniform float uTime; 
uniform vec2 firePos;
uniform float fireRadius;

void main() {
    float t = fract(uTime * 0.2);  // za menjanje tekstura
    
    vec4 texColor1 = texture(uTex1, chTex); //za svaku poziciju uzima "boju" rgba teksture
    vec4 texColor2 = texture(uTex2, chTex);
    vec4 texColor3 = texture(uTex3, chTex);
    
    vec4 texColor;
    if (t < 0.33) {
        //treci param, 0-1, 0-uzima ceo x, 1-ceo y
        texColor = mix(texColor1, texColor2, t * 3.0); //dok se t krece od 0 do 0.33 mesaj te dve
    } else if (t < 0.66) {
        texColor = mix(texColor2, texColor3, (t - 0.33) * 3.0);
    } else {
        texColor = mix(texColor3, texColor1, (t - 0.66) * 3.0);
    }
    
    float dist = length(fragPos - firePos);

    float lightIntensity = clamp(1.0 - (dist / fireRadius), 0.0, 1.0);

    vec4 lightColor = vec4(0.737254902, 0.0, 0.176470588, lightIntensity);

    outCol = texColor + lightColor * lightIntensity;
}
