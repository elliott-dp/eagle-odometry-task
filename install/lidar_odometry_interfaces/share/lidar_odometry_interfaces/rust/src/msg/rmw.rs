#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "lidar_odometry_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__lidar_odometry_interfaces__msg__ProcessingStats() -> *const std::ffi::c_void;
}

#[link(name = "lidar_odometry_interfaces__rosidl_generator_c")]
extern "C" {
    fn lidar_odometry_interfaces__msg__ProcessingStats__init(msg: *mut ProcessingStats) -> bool;
    fn lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ProcessingStats>, size: usize) -> bool;
    fn lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ProcessingStats>);
    fn lidar_odometry_interfaces__msg__ProcessingStats__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ProcessingStats>, out_seq: *mut rosidl_runtime_rs::Sequence<ProcessingStats>) -> bool;
}

// Corresponds to lidar_odometry_interfaces__msg__ProcessingStats
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// added preprocessing and registration to better analyze the stats and find botttlenecks

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ProcessingStats {
    /// stamp = timestamp della scansione
    pub header: std_msgs::msg::rmw::Header,

    /// how many points in the raw scan
    pub input_points: u32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub filtered_points: u32,

    /// conversion from ROS, NaN removal and range crop
    pub crop_ms: f64,

    /// transform to base_link, ground fit, levelling and ground removal
    pub ground_ms: f64,

    /// coarse and fine voxel grids
    pub downsample_ms: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub preprocessing_ms: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub registration_ms: f64,

    /// adding the fine cloud to the voxel map
    pub map_update_ms: f64,

    /// odometry, TF, markers, filtered cloud and map
    pub publish_ms: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub total_ms: f64,

}



impl Default for ProcessingStats {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !lidar_odometry_interfaces__msg__ProcessingStats__init(&mut msg as *mut _) {
        panic!("Call to lidar_odometry_interfaces__msg__ProcessingStats__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ProcessingStats {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { lidar_odometry_interfaces__msg__ProcessingStats__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ProcessingStats {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ProcessingStats where Self: Sized {
  const TYPE_NAME: &'static str = "lidar_odometry_interfaces/msg/ProcessingStats";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__lidar_odometry_interfaces__msg__ProcessingStats() }
  }
}


