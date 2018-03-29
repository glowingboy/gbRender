return
   {
      Name = "scene_0",
      Children =
	 {
	    {
	       Name = "camera",
	       Transform =
		  {
			Scale = {1.0, 1.0, 1.0},
			Rotation = {0.0, 0.0, 0.0},
			Position = {0.0, 0.0, -10.0}
		  },
	       Elements = 
		  {
		     {
			Type = ElementType.Camera,
			RenderQueue = RenderQueue.Background
		     }
		  }
	    },
	    {
	       Name = "triangle",
	       Transform =
		  {
		     Scale = {1.0, 1.0, 1.0},
		     Rotation = {0.0, 0.0, 0.0},
		     Position = {0.0, 0.0, 0.0}
		  },
	       Elements =
		  {
		     {
			Type = ElementType.Render,
			Mesh = "triangle.lua",
			Material = "tint.lua",
		     }
		  }
	    },
	    {
	       Name = "parent",
	       Transform =
		  {
		     Scale = {1.0, 1.0, 1.0},
		     Rotation = {0.0, 0.0, 0.0},
		     Position = {0.0, 0.0, 0.0}
		  },
	       Children =
		  {
		     {
			Name = "child",
			Transform =
			   {
			      Scale = {1.0, 1.0, 1.0},
			      Rotation = {0.0, 0.0, 0.0},
			      Position = {0.0, 0.0, 0.0}	
			   },
			Elements =
			   {
			      {
				 Type = ElementType.Render,
				 Mesh = "triangle.lua",
				 Material = "tinit.lua"
			      }
			   }
		     }
		  }
	    }
	 }
   }
