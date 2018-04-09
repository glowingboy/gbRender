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
				 ClearColor = {0.0, 0.0, 1.0, 1.0}
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
				 ClearColor = {1.0, 0.0, 0.0, 1.0}
			      }
			   }
		     }
		  }
	    }
	 }
   }
