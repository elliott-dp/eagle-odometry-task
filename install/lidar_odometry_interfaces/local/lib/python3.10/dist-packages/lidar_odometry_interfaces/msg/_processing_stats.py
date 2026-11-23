# generated from rosidl_generator_py/resource/_idl.py.em
# with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ProcessingStats(type):
    """Metaclass of message 'ProcessingStats'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('lidar_odometry_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'lidar_odometry_interfaces.msg.ProcessingStats')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__processing_stats
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__processing_stats
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__processing_stats
            cls._TYPE_SUPPORT = module.type_support_msg__msg__processing_stats
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__processing_stats

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ProcessingStats(metaclass=Metaclass_ProcessingStats):
    """Message class 'ProcessingStats'."""

    __slots__ = [
        '_header',
        '_input_points',
        '_filtered_points',
        '_crop_ms',
        '_ground_ms',
        '_downsample_ms',
        '_preprocessing_ms',
        '_registration_ms',
        '_map_update_ms',
        '_publish_ms',
        '_total_ms',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'input_points': 'uint32',
        'filtered_points': 'uint32',
        'crop_ms': 'double',
        'ground_ms': 'double',
        'downsample_ms': 'double',
        'preprocessing_ms': 'double',
        'registration_ms': 'double',
        'map_update_ms': 'double',
        'publish_ms': 'double',
        'total_ms': 'double',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.input_points = kwargs.get('input_points', int())
        self.filtered_points = kwargs.get('filtered_points', int())
        self.crop_ms = kwargs.get('crop_ms', float())
        self.ground_ms = kwargs.get('ground_ms', float())
        self.downsample_ms = kwargs.get('downsample_ms', float())
        self.preprocessing_ms = kwargs.get('preprocessing_ms', float())
        self.registration_ms = kwargs.get('registration_ms', float())
        self.map_update_ms = kwargs.get('map_update_ms', float())
        self.publish_ms = kwargs.get('publish_ms', float())
        self.total_ms = kwargs.get('total_ms', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.input_points != other.input_points:
            return False
        if self.filtered_points != other.filtered_points:
            return False
        if self.crop_ms != other.crop_ms:
            return False
        if self.ground_ms != other.ground_ms:
            return False
        if self.downsample_ms != other.downsample_ms:
            return False
        if self.preprocessing_ms != other.preprocessing_ms:
            return False
        if self.registration_ms != other.registration_ms:
            return False
        if self.map_update_ms != other.map_update_ms:
            return False
        if self.publish_ms != other.publish_ms:
            return False
        if self.total_ms != other.total_ms:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def input_points(self):
        """Message field 'input_points'."""
        return self._input_points

    @input_points.setter
    def input_points(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'input_points' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'input_points' field must be an unsigned integer in [0, 4294967295]"
        self._input_points = value

    @builtins.property
    def filtered_points(self):
        """Message field 'filtered_points'."""
        return self._filtered_points

    @filtered_points.setter
    def filtered_points(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'filtered_points' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'filtered_points' field must be an unsigned integer in [0, 4294967295]"
        self._filtered_points = value

    @builtins.property
    def crop_ms(self):
        """Message field 'crop_ms'."""
        return self._crop_ms

    @crop_ms.setter
    def crop_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'crop_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'crop_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._crop_ms = value

    @builtins.property
    def ground_ms(self):
        """Message field 'ground_ms'."""
        return self._ground_ms

    @ground_ms.setter
    def ground_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ground_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'ground_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._ground_ms = value

    @builtins.property
    def downsample_ms(self):
        """Message field 'downsample_ms'."""
        return self._downsample_ms

    @downsample_ms.setter
    def downsample_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'downsample_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'downsample_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._downsample_ms = value

    @builtins.property
    def preprocessing_ms(self):
        """Message field 'preprocessing_ms'."""
        return self._preprocessing_ms

    @preprocessing_ms.setter
    def preprocessing_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'preprocessing_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'preprocessing_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._preprocessing_ms = value

    @builtins.property
    def registration_ms(self):
        """Message field 'registration_ms'."""
        return self._registration_ms

    @registration_ms.setter
    def registration_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'registration_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'registration_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._registration_ms = value

    @builtins.property
    def map_update_ms(self):
        """Message field 'map_update_ms'."""
        return self._map_update_ms

    @map_update_ms.setter
    def map_update_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'map_update_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'map_update_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._map_update_ms = value

    @builtins.property
    def publish_ms(self):
        """Message field 'publish_ms'."""
        return self._publish_ms

    @publish_ms.setter
    def publish_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'publish_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'publish_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._publish_ms = value

    @builtins.property
    def total_ms(self):
        """Message field 'total_ms'."""
        return self._total_ms

    @total_ms.setter
    def total_ms(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'total_ms' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'total_ms' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._total_ms = value
