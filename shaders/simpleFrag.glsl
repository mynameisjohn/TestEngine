#version 130

precision mediump float;

varying vec2 v_TexCoord;
uniform sampler2D u_Texture;

uniform vec4 u_Color;

void main(){
	//Should multiply by black and white texture
	gl_FragColor = u_Color * texture2D(u_Texture, v_TexCoord);
}
