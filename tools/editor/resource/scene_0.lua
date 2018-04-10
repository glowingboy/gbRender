return
   {
      Name = "Editor",
      Children =
	 {
	    {
	       Name = "UI",
	       Children = 
		  {
		     {
			Name = "CameraUI",
			Elements =
			   {
			      {
				 Type = ElementType.Camera,
				 RenderQueue = RenderQueue.UI,
				 ClearColor = {0.0, 0.0, 0.3, 1.0}
			      }
			   }
		     }  
		  }
	    },
	    {
	       Name = "Scene",
	       Children =
		  {
		     {
			Name = "CameraScene",
			Elements =
			   {
			      {
				 Type = ElementType.Camera,
				 RenderQueue = RenderQueue.Opaque,
				 ClearColor = {0.3, 0.0, 0.0, 1.0}
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
				 Material = "Tint.gbShader"
			      }
			   }
		     }
		  }
	    }
	 }
   }
