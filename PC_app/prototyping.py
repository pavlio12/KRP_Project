# Just for testing


import numpy as np

## 170° error near the 0°
reference = np.deg2rad(170)
actual = np.deg2rad(0)

s_ref = np.sin(reference)
c_ref = np.cos(reference)

s_act = np.sin(actual)
c_act = np.cos(actual)

sin_err  =  s_ref * c_act - c_ref * s_act
cos_err  =  c_ref * c_act + s_ref * s_act
angle_err = np.arctan2(sin_err, cos_err)

print(np.rad2deg(angle_err))


## 190° error near the 0°
reference = np.deg2rad(190)
actual = np.deg2rad(5)

s_ref = np.sin(reference)
c_ref = np.cos(reference)

s_act = np.sin(actual)
c_act = np.cos(actual)

sin_err  =  s_ref * c_act - c_ref * s_act
cos_err  =  c_ref * c_act + s_ref * s_act
angle_err = np.arctan2(sin_err, cos_err)

print(np.rad2deg(angle_err))




### Continuous Controller
# Init
prev_error = 0.0

# STEP 1
## 170° error near the 0°
reference = np.deg2rad(170)
actual = np.deg2rad(0)

s_ref = np.sin(reference)
c_ref = np.cos(reference)

s_act = np.sin(actual)
c_act = np.cos(actual)

sin_err  =  s_ref * c_act - c_ref * s_act
cos_err  =  c_ref * c_act + s_ref * s_act
angle_err = np.arctan2(sin_err, cos_err)

# Unwrap: adjust to be closest to previous error
err_diff = angle_err - prev_error

# If the difference is > π, we crossed the discontinuity
if err_diff > np.pi:
    angle_err -= 2 * np.pi
elif err_diff < -np.pi:
    angle_err += 2 * np.pi

prev_error = angle_err
print(np.rad2deg(angle_err))



# STEP 2
## 190° error near the 0°
reference = np.deg2rad(190)
actual = np.deg2rad(5)

s_ref = np.sin(reference)
c_ref = np.cos(reference)

s_act = np.sin(actual)
c_act = np.cos(actual)

sin_err  =  s_ref * c_act - c_ref * s_act
cos_err  =  c_ref * c_act + s_ref * s_act
angle_err = np.arctan2(sin_err, cos_err)

# Unwrap: adjust to be closest to previous error
err_diff = angle_err - prev_error

# If the difference is > π, we crossed the discontinuity
if err_diff > np.pi:
    angle_err -= 2 * np.pi
elif err_diff < -np.pi:
    angle_err += 2 * np.pi

prev_error = angle_err
print(np.rad2deg(angle_err))