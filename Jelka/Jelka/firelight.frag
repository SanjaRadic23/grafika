#version 330 core
out vec4 outCol;

uniform vec2 fireCenter; // Pozicija centra vatre (u prostoru šejdera)
uniform float maxDistance; // Maksimalna udaljenost za potpuni efekat transparentnosti
uniform float minAlpha;   // Minimalna po?etna transparentnost (na primer, 0.01 za skoro providan centar)
uniform float time;  

void main()
{
    // Normalizovanje koordinata trenutnog fragmenta u opseg (-1, 1)
    vec2 centered = gl_FragCoord.xy / vec2(1500.0, 800.0);  // Veli?ina ekrana - prilagodite prema vašem slu?aju
    centered = centered * 2.0 - 1.0;  // Premesti u (-1, 1) prostor
    
    // Izra?unavanje udaljenosti od centra
    float distanceFromCenter = length(centered - fireCenter);
    
    // Normalizovanje transparentnosti na osnovu udaljenosti od centra
    // Po?etna transparentnost je minAlpha, a smanjuje se prema ivici
    float alpha = clamp(minAlpha + (1.0 - minAlpha) * (1.0 - (distanceFromCenter / maxDistance)), 0.0, 1.0);
    
    float flicker = 1.0 + 0.1 * sin(time * 3.0);

    // Boja i providnost
    vec3 color = vec3(1.0, 0.647, 0.0); // Možete promeniti boju ako želite
    outCol = vec4(color * flicker, alpha);
}
