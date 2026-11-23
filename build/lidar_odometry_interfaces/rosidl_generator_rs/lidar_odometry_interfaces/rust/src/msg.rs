#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to lidar_odometry_interfaces__msg__ProcessingStats
/// added preprocessing and registration to better analyze the stats and find botttlenecks

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ProcessingStats {
    /// stamp = timestamp della scansione
    pub header: std_msgs::msg::Header,

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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ProcessingStats::default())
  }
}

impl rosidl_runtime_rs::Message for ProcessingStats {
  type RmwMsg = super::msg::rmw::ProcessingStats;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        input_points: msg.input_points,
        filtered_points: msg.filtered_points,
        crop_ms: msg.crop_ms,
        ground_ms: msg.ground_ms,
        downsample_ms: msg.downsample_ms,
        preprocessing_ms: msg.preprocessing_ms,
        registration_ms: msg.registration_ms,
        map_update_ms: msg.map_update_ms,
        publish_ms: msg.publish_ms,
        total_ms: msg.total_ms,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      input_points: msg.input_points,
      filtered_points: msg.filtered_points,
      crop_ms: msg.crop_ms,
      ground_ms: msg.ground_ms,
      downsample_ms: msg.downsample_ms,
      preprocessing_ms: msg.preprocessing_ms,
      registration_ms: msg.registration_ms,
      map_update_ms: msg.map_update_ms,
      publish_ms: msg.publish_ms,
      total_ms: msg.total_ms,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      input_points: msg.input_points,
      filtered_points: msg.filtered_points,
      crop_ms: msg.crop_ms,
      ground_ms: msg.ground_ms,
      downsample_ms: msg.downsample_ms,
      preprocessing_ms: msg.preprocessing_ms,
      registration_ms: msg.registration_ms,
      map_update_ms: msg.map_update_ms,
      publish_ms: msg.publish_ms,
      total_ms: msg.total_ms,
    }
  }
}


