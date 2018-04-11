return
   {
      Name = "Editor",
      Children =
	 {
	    -- {
	    --    Name = "UI",
	    --    Children = 
	    -- 	  {
	    -- 	     {
	    -- 		Name = "CameraUI",
	    -- 		Elements =
	    -- 		   {
	    -- 		      {
	    -- 			 Type = ElementType.Camera,
	    -- 			 RenderQueue = RenderQueue.UI,
	    -- 			 ClearColor = {0.0, 0.0, 0.3, 1.0}
	    -- 		      }
	    -- 		   }
	    -- 	     }  
	    -- 	  }
	    -- },
	    {
	       Name = "Scene",
	       Children =
		  {
		     {
			Name = "CameraScene",
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
				 RenderQueue = RenderQueue.Opaque,
			      }
			   }
		     },
		     {
			Name = "test",
			Elements =
			   {
			      {
				 Type = ElementType.Render,
				 Mesh = "Triangle.lua",
				 Material = "Tint.lua"
			      }
			   }
		     }
		  }
	    }
	 }
   }
