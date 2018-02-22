return
   {
      Name = "test_root",
      Children =
	 {
	    {
	       Name = "test0",
	       Elements = 
		  {
		     {
			Type = 2, -- render
			Mesh = "mesh.path",
			Material = "mat.path"
		     }
		  }
	    },
	    {
	       Name = "test1",
	       Elements =
		  {}
	    },
	    {
	       Name = "test2",
	       Elements =
		  {},
	       Children =
		  {
		     {
			Name = "t2_c0"
		     },
		     {
			Name = "t2_c1"
		     }
		  }
	    }
	 }
   }
