#version 330 core
out vec4 FragColor;

in vec2 fragCoord;  // Koordinate fragmenta unutar ta?ke
in vec3 chCol;      // Jedinstvena boja za svaku lampicu

uniform float time;
uniform int mode;
uniform float lampRadius; // Radijus osvetljenja lampice

void main() {
    // Koordinate unutar ta?ke (0.0 do 1.0)
    vec2 pointCoord = gl_PointCoord - vec2(0.5);
    
    // Izra?unavanje udaljenosti od centra ta?ke (od 0.0 do 0.5)
    float dist = length(pointCoord);
    
    // Odbacivanje fragmenata koji su van kružnog oblika
    if (dist > 0.5) {
        discard;
    }

    // Intenzitet osvetljenja: najja?i u centru, slabi prema ivici
    float lightIntensity = 0.5 + 0.5 * (1.0 - dist / lampRadius);

    float alpha = 0.5 + 0.5 * (1.0 - dist / 0.5);
    
    // Koli?ina svetlosti oko lampice
    vec3 lightEffect = chCol * lightIntensity * alpha; // Kombinacija boje lampice i svetlosti
    
    // Ako je mod 1 ili 2, osvetljenje se menja na osnovu treptanja
    if (mode == 1) {
        float intensity = 0.5 + 0.5 * sin(time * 5.0);  // Stati?ko treptanje
        lightEffect *= intensity;
    } else if (mode == 2) {
        float randomBlink = step(0.1, fract(sin(time * 500.0)));  // Slu?ajno treptanje
        lightEffect *= randomBlink;
    }
    
    // Kombinacija boje lampice sa svetlosnim efektom
    FragColor = vec4(lightEffect, alpha);  // Postavljanje rezultuju?e boje sa transparentnoš?u
}
