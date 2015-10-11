#version 330 core
precision highp float;

// Vertex shader for instancing of colored cubes
// "v_vertex" contains one of the vertices of the cube
// "v_normal" contains the normal of that side of the cube
// "instance_position" contains the cube center in xyz and size in w
// "instance_rotation" contains the cube rotation as a quaternion
// "instance_color" contains the cube color in rgba values ranged 0.0~1.0
layout (location = 0) in vec4 v_vertex;
layout (location = 1) in vec4 v_normal;
layout (location = 2) in vec4 instance_position;
layout (location = 3) in vec4 instance_rotation;
layout (location = 4) in vec4 instance_diffuse;
layout (location = 5) in float instance_specular;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

out Fragment {
    smooth vec4 diffuse;
    smooth vec4 specular;
    vec3 normal;
    smooth float depth;
     vec3 pos;
} fragment;

// Quaternion rotation
vec4 multQ( vec4 oq, vec4 rq ) {
	// The constructor takes its arguments as (x, y, z, w)
	return vec4(oq.w * rq.x + oq.x * rq.w + oq.y * rq.z - oq.z * rq.y,
                oq.w * rq.y + oq.y * rq.w + oq.z * rq.x - oq.x * rq.z,
	            oq.w * rq.z + oq.z * rq.w + oq.x * rq.y - oq.y * rq.x,
	            oq.w * rq.w - oq.x * rq.x - oq.y * rq.y - oq.z * rq.z);
}
vec4 getConjugate( vec4 q ) { return vec4(-q.x, -q.y, -q.z, q.w); }

// Multiplying a quaternion q with a vector v applies the q-rotation to v
vec3 rotate(vec3 vert, vec4 rot) {
    float len = length(vert);
    // Normalizing a vector of (0,0,0) will fail
    if ( len == 0.0 ) return vec3(0,0,0);
    vec3 vn = vert/len; // Manual normalization to make use of length calculation
    
	vec4 vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
    
	resQuat = multQ( vecQuat, getConjugate(rot) );
	resQuat = multQ( rot, resQuat );
    // We have to multiply by the length of original vert to maintain scale
	return vec3(resQuat.x, resQuat.y, resQuat.z)*len;
}

void main(void) {
    // Scale vertex - size of instance from instance_position.w
    vec3 vertex = v_vertex.xyz*instance_position.w;
    // Rotate vertex
    vertex = rotate( vertex, instance_rotation);
    // Move vertex
    vertex += instance_position.xyz;
    // Rotate normal
    vec3 normal = rotate( normalize( v_normal.xyz ), instance_rotation );

    
    gl_Position = MVP*(vec4(vertex.xyz,1));
    fragment.pos = vertex.xyz;
    
    // v_vertex.w contains vertex light intensity
    // (for minecraft-style ambient occlusion)
    fragment.diffuse = vec4(instance_diffuse.rgb*v_vertex.w, instance_diffuse.a);
    fragment.specular = vec4(vec3(instance_specular),instance_diffuse.a);
    fragment.normal = normal;
    fragment.depth = gl_Position.z;
}
