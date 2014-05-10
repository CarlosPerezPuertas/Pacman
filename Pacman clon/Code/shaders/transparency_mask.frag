uniform sampler2D texture;
uniform vec4 colorkey;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    if (pixel == colorkey)
        pixel.a = 0;
    gl_FragColor = pixel;
}