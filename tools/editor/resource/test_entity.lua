return
   {
      Name = "test_root",
      Children =
	 {
	    {
	       Name = "test0",
	       Transform =
		  {
			Scale = {1.0, 2.0, 3.0},
			Rotation = {0.1, 0.2, 0.3},
			Position = {1.23, 4.56, 7.89}
		  },
	       Elements = 
		  {
		     {
			Type = 2, -- render
			Mesh = "test_mesh.lua",
			Material = "mat.path"
		     }
		  }
	    },
	    {
	       Name = "test1",
	       Transform =
		  {
		     Scale = {1.0, 1.0, 1.0},
		     Rotation = {0.1, 0.2, 0.3},
		     Position = {1.23, 4.56, 7.89}
		  }
	    },
	    {
	       Name = "test2",
	       Transform =
		  {
		     Scale = {1.0, 2.0, 3.0},
		     Rotation = {0.0, 0.0, 0.0},
		     Position = {1.23, 4.56, 7.89}
		  },
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
