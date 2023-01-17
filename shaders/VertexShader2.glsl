#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform vec3 CarP;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_texture;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color;
    frag_position = v_position;
    frag_normal = v_normal  + vec3(cos(Time), 1, 0);
    frag_texture = v_texture;

    vec3 world_pos = (Model* vec4(v_position, 1.0)).xyz;
    float dist = distance(CarP, world_pos);
    world_pos.y -= dist*dist*0.003;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View  * vec4(world_pos, 1.0);

}
