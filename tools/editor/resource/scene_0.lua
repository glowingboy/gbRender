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
			Name = "Main",
			Elements =
			   {
			      {
				 Type = ElementType.Main
			      }
			   }
		     },
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
				 Frustum =
				    {
				       60.0, 640.0 / 480.0, 0.1, 100
				    }
			      },
			      {
				 Type = ElementType.FPVController
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
		     },
		     {
			Name = "text",
			Elements =
			   {
			      {
				 Type = ElementType.Text,
				 Material = "SDFText.lua",
				 Text = "hello world!"
			      }
			   }
		     },
		     {
			Name = "texture",
			Elements =
			   {
			      {
				 Type = ElementType.Texture,
				 Material = "SDFText.lua"
			      }
			   }
		     }
		  }
	    }
	 }
   }
