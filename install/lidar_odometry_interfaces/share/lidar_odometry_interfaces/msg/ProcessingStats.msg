# added preprocessing and registration to better analyze the stats and find botttlenecks

std_msgs/Header header       	# stamp = timestamp della scansione
uint32 input_points		# how many points in the raw scan
uint32 filtered_points
float64 crop_ms           # conversion from ROS, NaN removal and range crop
float64 ground_ms         # transform to base_link, ground fit, levelling and ground removal
float64 downsample_ms     # coarse and fine voxel grids
float64 preprocessing_ms	
float64 registration_ms
float64 map_update_ms     # adding the fine cloud to the voxel map
float64 publish_ms        # odometry, TF, markers, filtered cloud and map
float64 total_ms