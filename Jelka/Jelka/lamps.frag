#version 330 core

out vec4 FragColor;

in vec2 fragCoord;  // koordinate frag
in vec3 chCol;      // boja tog frag

uniform float time; //uniformne prosledjujemo iz .cpp
uniform int mode;
uniform float lampRadius; // radijus za osvetljenje

void main() {
    vec2 pointCoord = gl_PointCoord - vec2(0.5); //centar tacke 0.5 0.5, od fragmenta (kvadrat) oduzmemo
    
    float dist = length(pointCoord); //uzmem kolika je razdaljina
    
    if (dist > 0.5) { //sve preko 0.5 odbacujem (dobijam krug)
        discard;
    }

    float lightIntensity = 0.5 + 0.5 * (1.0 - dist / lampRadius); //udaljavanjem fragmenta od centra svetlost opada
    //0.5-1.0

    float alpha = 0.5 + 0.5 * (1.0 - dist / 0.5); //kanal za transparentnost isto 0.5-1.0
    
    vec3 lightEffect = chCol * lightIntensity * alpha; //boja, osvetljenost, providnost -> kombinuj
    //alpha ovde samo utice na samu boju, ali mi ne znamo u ovom koraku koliko je fragment providan
    
    if (mode == 1) {
        float intensity = 0.5 + 0.5 * sin(time * 5.0);  //time je za brzinu
        lightEffect *= intensity;
    } else if (mode == 2) {
        float randomBlink = step(0.1, fract(sin(time * 500.0))); //brzo osciluje *500, uzimaj decimalne vrednosti fract
        //0.1 je prag iznad kog svetlost blinka
        lightEffect *= randomBlink;
    }
    
    FragColor = vec4(lightEffect, alpha); //kombinuj, prosledjujemo rgb, alpha, tu treba da znamo koliko je providan frag
}
