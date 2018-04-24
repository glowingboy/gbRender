ElementType = 
   {
      Default = 0,
      Camera = 1,
      Render = 2,
      Mesh = 3,
      Texture = 4,
      Text = 5,
      FPVController = 6,
      EntityPicker = 7
   }

function ElementType:Register(types)
   for k, v in pairs(types) do
      if self[k] == nil then
	 self[k] = v
      else
	 print("k already in ElementType, k@: " .. k)
      end
   end
end
