import json

#para usar como uma função separada tem de se fazer o seguinte comando "from convert import jsonconvert"
#este codigo passa uma entrada ubinascii para utf-8 que depois é introduzido no array json

def jsonconvert(arr):
	arr2=[]
	l=len(arr)
	i=0

	
	while i < l:
		y=0
		y=arr[i].decode('utf-8')
		t='"'+y+'"'
		arr2.append(y)
		
		i+=1
		
	x= {"mac": arr2 }
	y=json.dumps(x)
	
	
	return y


