#version 330 core
out vec4 outCol;

uniform vec2 fireCenter; // centar vatre
uniform float maxDistance; // max udaljenost gde je skroz transparentno
uniform float minAlpha;   // min providnost od koje se pocinje
uniform float time;  

void main()
{
    // gl_FragCoord.xy vraca bukvalno lokaciju piksela na ekranu, a mi radimo 0.0-1.0
    vec2 centered = gl_FragCoord.xy / vec2(1500.0, 800.0);
    centered = centered * 2.0 - 1.0;  // -1 do 1
    
    // udaljenost ivice frag od centra vatre
    float distanceFromCenter = length(centered - fireCenter);
    
    // providnost treba da se povecava od centra ka ivici, 0-1                                          
    float alpha = clamp(minAlpha + (1.0 - minAlpha) * (1.0 - (distanceFromCenter / maxDistance)), 0.0, 1.0);
    
    //postizanje blagog treperenje trzanjem sinusa
    float flicker = 1.0 + 0.1 * sin(time * 3.0);

    vec3 color = vec3(1.0, 0.647, 0.0); // boja osvetljenja
    outCol = vec4(color * flicker, alpha); //flicker i boja zajedno i to se kombinuje sa providnoscu
}
