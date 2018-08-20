#version 130
#extension GL_EXT_texture_array: enable

uniform sampler2DArray _terrain_h;
uniform sampler2DArray _terrain_d;
uniform sampler2DArray _colors;
uniform sampler2DArray _interest;

uniform int _displayMode;

vec3 DX = vec3(1.0/1024.0, 0.0, 0.0);
vec3 DY = vec3(0.0, 1.0/1024.0, 0.0);


varying vec2 _v_texCoords;
varying vec3 _v_normal;
varying vec4 _v_pos;


vec3 sun = vec3(3000.0, 3000.0, 500.0);
vec3 tex3Dcoord;
void main(void)
{
        vec4 color;
        color.a = 1.0;
        
        tex3Dcoord = vec3 (_v_texCoords.xy,0.0);
        float water = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;
        
        tex3Dcoord = vec3 (_v_texCoords.xy,1.0);
        float vegetation = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;
        
        tex3Dcoord = vec3 (_v_texCoords.xy,2.0);
        float roads = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;
	vec3 roadsCol = texture2DArray(_terrain_d, tex3Dcoord.xyz).rgb;
        float roadType = texture2DArray(_terrain_d, tex3Dcoord.xyz).g;
        //roads = 0;

        tex3Dcoord = vec3 (_v_texCoords.xy,3.0);
        float rock = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;

        tex3Dcoord = vec3 (_v_texCoords.xy,4.0);
        float snow = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;

        tex3Dcoord = vec3 (_v_texCoords.xy,5.0);
        float houses = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;
        float housestype = texture2DArray(_terrain_d, tex3Dcoord.xyz).g;

        tex3Dcoord = vec3 (_v_texCoords.xy,6.0);
        float walls = texture2DArray(_terrain_d, tex3Dcoord.xyz).r;
        
        vec3 normal;
        normal = _v_normal;
                
        float slop = dot( vec3(0,1,0), normal);
        
	float light = dot(normalize(sun), normal);
	vec3 vlight = vec3(light, light, light);
	                
                
        // NORMAL
        if ( _displayMode == 0) 
        {            
                tex3Dcoord = vec3 (_v_texCoords.xy * 128.0, 3.0);
                vec3 colorsnow = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                
                tex3Dcoord = vec3 (_v_texCoords.xy * 16.0, 1.0);
                vec3 colorrock = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                
                tex3Dcoord = vec3 (_v_texCoords.xy * 256.0, 5.0);
                vec3 colorveg = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                
                tex3Dcoord = vec3 (_v_texCoords.xy * 16.0, 4.0);
                vec3 colorwater = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                
                tex3Dcoord = vec3 (_v_texCoords.xy * 256.0, 0.0);
                vec3 colordef = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                //colorveg = colordef;
                colordef = 0.5 * colorveg + 0.5 * colorveg;
                
                color.rgb = colordef;
                color.rgb = (1.0 - snow) * color.rgb  + snow * colorsnow;
                color.rgb = (1.0 - rock) * color.rgb  + rock * colorrock;
                color.rgb = (1.0 - vegetation) * color.rgb  + vegetation * colorveg;
                color.rgb = (1.0 - water) * color.rgb  + water * colorwater;
                color.rgb *= vlight;
                
                if (houses > 2.1)
                {
                        if (housestype < 0.1)
                                color.rgb = vec3(0.4,0.4,0.9);
                        else if (housestype < 0.2)
                                color.rgb = vec3(0.4,0.4,0.0);
                        else if (housestype < 0.3)
                                color.rgb = vec3(0.9,0.4,0.9);
                        else if (housestype < 0.4)
                                color.rgb = vec3(0.4,0.9,0.9);
                }  
                if (walls > 0.2)
                {
                        color.rgb = vec3(0.2,0.2,0.2);
                }    	       
                
                if (slop < 0.55)
                {                       
                        color.rgb = colorrock;
                }
                else if (slop < 0.75)
                {                    
                        float t = 1.0 - (slop - 0.55)*5.0;   
                        color.rgb =  t  * colorrock + (1.0 - t) * color.rgb;
                }
                  
		// ROADS
	        {        
                        tex3Dcoord = vec3 (_v_texCoords.xy * 512.0, 7.0);
		        vec3 colorroad = texture2DArray(_colors, tex3Dcoord.xyz).rgb;
		        color.rgb = (1.0-roads) *  color.rgb
			        + roads * colorroad;
	        }
        }
        // IGN
        else if ( _displayMode == 1) 
        {
		// GRAY
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		color.rgb = 0.5 + 0.5 * texture2DArray(_terrain_h, tex3Dcoord.xyz).rgb;
		//color.rgb *= vlight;	
		
                vec3 colorsnow  = vec3(1.0, 1.0, 1.0);
                vec3 colorrock  = vec3(0.2, 0.2, 0.2);
                vec3 colorveg   = vec3(0.0, 0.9, 0.0);
                vec3 colorwater = vec3(0.0, 0.5, 1.0);
                
                //color.rgb = (1.0 - snow) * color.rgb  + snow * colorsnow;
                //color.rgb = (1.0 - rock) * color.rgb  + rock * colorrock;
                //color.rgb = (1.0 - vegetation) * color.rgb  + vegetation * colorveg;
                //color.rgb = (1.0 - water) * color.rgb  + water * colorwater;
                //color.rgb *= vlight;
	                
		
                if (water > 0.5)
                {
                        color.rgb = texture2DArray(_terrain_h, tex3Dcoord.xyz).rgb;
                        tex3Dcoord = vec3 (_v_texCoords.xy * 16.0, 4.0);
                        color.rgb = color.rgb * 0.5 + 0.5 * texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                }  
                
		// ROAD
		if (roads > 0.1)
		{
			/*
				vec3 colorroad = vec3(0.7,roadType,0.0);
				color.rgb = (1.0-roads) *  color.rgb
					+ roads * colorroad;
			*/
			color.rgb = roadsCol;
		}

                
                if (houses > 0.0)
                {
                        if (housestype < 0.01)
                                color.rgb = vec3(0.4,0.9,0.4);
                        else if (housestype < 0.05)
                                color.rgb = vec3(0.0,0.9,0.0);
                        else if (housestype < 0.1)
                                color.rgb = vec3(0.0,0.0,0.9);
                        else if (housestype < 0.15)
                                color.rgb = vec3(0.9,0.9,0.9);
                        else 
                                color.rgb = vec3(0.4,0.9,0.9);
                }  
                if (walls > 0.0)
                {
                        color.rgb = vec3(0.2,0.2,0.2);
                }    
		
		// LINES
		float levelSize = 10.0;

		int h = int(_v_pos.y / levelSize);	
		if ( abs((_v_pos.y/ levelSize)-h) < 0.05)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 		    	    



		// BUILDINGS
		tex3Dcoord = vec3 (_v_texCoords.xy,1.0);
		vec3 id = texture2DArray(_terrain_h, tex3Dcoord.xyz).rgb;		
		vec3 id2 = texture2DArray(_terrain_h, tex3Dcoord.xyz + DX).rgb;		
		vec3 id3 = texture2DArray(_terrain_h, tex3Dcoord.xyz + DY).rgb;	


		//color.rgb = vec3(1.0, 1.0, 1.0);

		if ( id.r > 0.0)

//		if ( ( abs(id.r-id2.r) > 0.)
			//     || ( abs(id.r-id3.r) > 0.) )
		{	 	
			color.r = id.g;
			color.g = id.r;
			color.b = id.b;
		}
		
	}
	// grid
        else if ( _displayMode == 2) 
	{



		// GRAY
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		color.rgb = 0.5 + 0.5 * texture2DArray(_terrain_h, tex3Dcoord.xyz).rgb;
		//color.rgb = vlight;	
		
		// ROAD
		vec3 colorroad = vec3(0.7,0.0,0.0);
		color.rgb = (1.0-roads) *  color.rgb
			+ roads * colorroad;

		float gridSize = 100.0;

		
               if (water > 0.5)
                {
                        color.rgb = texture2DArray(_terrain_h, tex3Dcoord.xyz).rgb;
                        tex3Dcoord = vec3 (_v_texCoords.xy * 16.0, 4.0);
                        color.rgb = color.rgb * 0.5 + 0.5 * texture2DArray(_colors, tex3Dcoord.xyz).rgb;
                }  
                
                
                if (houses > 0.1)
                {
                        color.rgb = vec3(0.4,0.4,0.9);
                }  
                
                
                if (walls > 0.2)
                {
                        color.rgb = vec3(0.2,0.2,0.2);
                }    


		// LINES
		float levelSize = 10.0;

		int h = int(_v_pos.y / levelSize);	
		if ( abs((_v_pos.y/ levelSize)-h) < 0.05)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 
                
		/*
		// GRID X
		int gx = int(_v_pos.x / gridSize);	
		if ( abs((_v_pos.x / gridSize)-gx) < 0.03)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 

		// GRID Z
		int gz = int(_v_pos.z / gridSize);	
		if ( abs((_v_pos.z / gridSize)-gz) < 0.03)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		}
		*/ 
	}
        // INTEREST
        
        else if ( _displayMode <= 12 || _displayMode >= 19) 
        {
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		float interest = texture2DArray(_interest, tex3Dcoord.xyz).r;
             		    
		// COLORMAP
		vec3 colorInterest;
		vec3 col1 = vec3(0.0, 0.0, 1.0);
		vec3 col2 = vec3(0.0, 1.0, 1.0);
		vec3 col3 = vec3(0.0, 1.0, 0.0);
		vec3 col4 = vec3(1.0, 1.0, 0.0);
		vec3 col5 = vec3(1.0, 0.0, 0.0);		
		
		if (interest < 0.25)
		{
			colorInterest = (1.0 - 4.0 * interest) * col1
				+ 4.0 * interest * col2;
		}
		else if (interest < 0.5)
		{
			colorInterest = (1.0 - (interest - 0.25) * 4.0) * col2
				+ ((interest - 0.25) * 4.0) * col3;
		}
		else if (interest < 0.75)
		{
			colorInterest = (1.0 - (interest - 0.5) * 4.0) * col3
				+ ((interest - 0.5) * 4.0) * col4;
		}
		else
		{
			colorInterest = (1.0 - (interest - 0.75) * 4.0) * col4
				+ ((interest - 0.75) * 4.0) * col5;
		}
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		color.rgb = colorInterest;

		float gridSize = 100.0;

		// GRID X
		int gx = int(_v_pos.x / gridSize);	
		if ( abs((_v_pos.x / gridSize)-gx) < 0.04)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 

		// GRID Z
		int gz = int(_v_pos.z / gridSize);	
		if ( abs((_v_pos.z / gridSize)-gz) < 0.04)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 
                
        }
        
        else if ( _displayMode >= 13 && _displayMode <= 18) 
        {
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		float interest = texture2DArray(_interest, tex3Dcoord.xyz).r;
             		    
		// COLORMAP
		vec3 colorInterest;
		/*	
		vec3 col1 = vec3(1.0, 0.0, 0.0);
		vec3 col2 = vec3(1.0, 0.0, 0.0);
		vec3 col3 = vec3(1.0, 1.0, 0.0);
		vec3 col4 = vec3(1.0, 1.0, 0.0);
		vec3 col5 = vec3(0.0, 1.0, 0.0);
		vec3 col6 = vec3(0.0, 0.3, 0.0);
		*/	
		vec3 col1 = vec3(0.0, 0.0, 1.0);
		vec3 col2 = vec3(0.0, 0.0, 1.0);
		vec3 col3 = vec3(0.0, 1.0, 1.0);
		vec3 col4 = vec3(0.0, 1.0, 0.0);
		vec3 col5 = vec3(1.0, 1.0, 0.0);
		vec3 col6 = vec3(1.0, 0.0, 0.0);
		
		if (interest < 0.2)
		{
			colorInterest = (1.0 - 5.0 * interest) * col1
				+ 5.0 * interest * col2;
		}
		else if (interest < 0.4)
		{
			colorInterest = (1.0 - (interest - 0.2) * 5.0) * col2
				+ ((interest - 0.2) * 5.0) * col3;
		}
		else if (interest < 0.6)
		{
			colorInterest = (1.0 - (interest - 0.4) * 5.0) * col3
				+ ((interest - 0.4) * 5.0) * col4;
		}
		else if (interest < 0.8)
		{
			colorInterest = (1.0 - (interest - 0.6) * 5.0) * col4
				+ ((interest - 0.6) * 5.0) * col5;
		}
		else 
		{
			colorInterest = (1.0 - (interest - 0.8) * 3.3) * col5
				+ ((interest - 0.8) * 5.0) * col6;	
		}
		tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
		color.rgb = colorInterest;

		float gridSize = 100.0;

		// GRID X
		int gx = int(_v_pos.x / gridSize);	
		if ( abs((_v_pos.x / gridSize)-gx) < 0.04)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 

		// GRID Z
		int gz = int(_v_pos.z / gridSize);	
		if ( abs((_v_pos.z / gridSize)-gz) < 0.04)
		{
			color *= vec4(0.6, 0.6, 0.6, 1.0);
		} 
                
        }
                
        
        if ( ! (_v_texCoords.x > 0.004
                && _v_texCoords.x < 0.996
                && _v_texCoords.y > 0.004
                && _v_texCoords.y < 0.996) )
        {  
                color = vec4(0.30, 0.30, 0.35, 1.0);                
        } 
        
        
        gl_FragColor = color;    

}

