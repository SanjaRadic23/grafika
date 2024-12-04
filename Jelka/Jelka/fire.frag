#version 330 core

in vec2 chTex;       // Teksturne koordinate
in vec2 fragPos;     // Pozicija fragmenta
out vec4 outCol;     // Izlazna boja

uniform sampler2D uTex1;  // Prva tekstura vatre
uniform sampler2D uTex2;  // Druga tekstura vatre
uniform sampler2D uTex3;  // Tre?a tekstura vatre
uniform float uTime;      // Trenutno vreme za animaciju
uniform vec2 firePos;     // Pozicija izvora vatre
uniform float fireRadius; // Maksimalni domet osvetljenja

void main() {
    // Podelite vreme na tri intervala
    float t = fract(uTime * 0.2);  // Brzo menjanje izme?u tekstura
    
    vec4 texColor1 = texture(uTex1, chTex);
    vec4 texColor2 = texture(uTex2, chTex);
    vec4 texColor3 = texture(uTex3, chTex);
    
    // Interpolacija izme?u tekstura
    vec4 texColor;
    if (t < 0.33) {
        texColor = mix(texColor1, texColor2, t * 3.0);
    } else if (t < 0.66) {
        texColor = mix(texColor2, texColor3, (t - 0.33) * 3.0);
    } else {
        texColor = mix(texColor3, texColor1, (t - 0.66) * 3.0);
    }
    
    // Izra?unavanje udaljenosti fragmenta od izvora vatre
    float dist = length(fragPos - firePos);

    float lightIntensity = clamp(1.0 - (dist / fireRadius), 0.0, 1.0);

    // Dodavanje efekta osvetljenja bele boje sa promenljivom transparentnoš?u
    vec4 lightColor = vec4(0.737254902, 0.0, 0.176470588, lightIntensity);

    // Kombinacija boje vatre i osvetljenja
    outCol = texColor + lightColor * lightIntensity;
}
