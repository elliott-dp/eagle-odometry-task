# added preprocessing and registration to better analyze the stats and find botttlenecks

std_msgs/Header header       	# stamp = timestamp della scansione
uint32 input_points		# how many points in the raw scan
uint32 filtered_points		
float64 preprocessing_ms	
float64 registration_ms
float64 total_ms
