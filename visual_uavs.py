import os
import matplotlib.pyplot as plt
import numpy as np

# function to load UAV flight data from a file

def load_uav_file(filename):
    times, xs, ys, azs = [], [], [], [] # lists to store time, x, y, azimuth data
    with open(filename, 'r') as f:
        next(f)  # skip the header line
        for line in f: 
            t, x, y, az = map(float, line.strip().split()) #split the line into time, x, y, azimuth 
            times.append(t)
            xs.append(x)
            ys.append(y)
            azs.append(az)
    return times, xs, ys, azs  

# function to load final target positions from a command file

def load_final_targets(cmd_file):
    final = {}
    with open(cmd_file, 'r') as f: #allows reading the command file
        for line in f:
            if not line.strip(): # skip empty lines
                continue
            t, uid, tx, ty = line.split() # split the line into time, uid, target x, target y
            uid = int(uid) 
            tx, ty = float(tx), float(ty)   
            final[uid] = (tx, ty) #take tx ty from the command
    return final

#make plots:

# default directory where the script is located
base_dir = os.path.dirname(__file__)  # get the directory of the current script

uav_folder = base_dir # folder containing UAV data files
cmds_filepath = os.path.join(base_dir, "src", "SimCmds.txt")  # construct the path to the command file assuming it is in the 'src' subdirectory

#define UAV IDs, colors, and arrow length for visualization

uav_ids = [] # list to store UAV IDs
# iterate through files in the UAV folder to find UAV data files
for filename in os.listdir(uav_folder):
    if filename.startswith("UAV") and filename.endswith(".txt"):
        try:   # check if the filename is in the expected format
            # extract the UAV ID from the filename
            uid = int(filename[3:-4])
            uav_ids.append(uid)
        except ValueError:
            continue
uav_ids.sort() # sort the UAV IDs for consistent ordering

# Generate colors dynamically
color_map = plt.get_cmap('tab10', len(uav_ids))  # use a colormap with enough distinct colors
colors = [color_map(i) for i in range(len(uav_ids))] #create a list of colors for each UAV

arrow_length = 5 # length of the arrows to indicate azimuth direction

#  Load the final target positions from the command file
final_targets = load_final_targets(cmds_filepath)

plt.figure(figsize=(6,6)) # create frame for the plot

# Loop through each UAV ID, load its data, and plot the flight path
for idx, uid in enumerate(uav_ids):
    color = colors[idx]
    uav_path = os.path.join(uav_folder, f"UAV{uid}.txt")
    times, xs, ys, azs = load_uav_file(uav_path)

    #plotting
    plt.plot(xs, ys, color=color, label=f"UAV {uid}") # plot the flight path with plot function

    #highlight the start and end points
    plt.scatter(xs[0], ys[0], marker='o', s=100, color=color) # marker for start point
    plt.text(xs[0], ys[0], f"Start {uid}", fontsize=6, va='bottom', ha='right')   
    plt.scatter(xs[-1], ys[-1], marker='x', s=100, color=color) #marker for end point
    plt.text(xs[-1], ys[-1], f"End {uid}", fontsize=6, va='top', ha='left')

    #add azimuth arrows along the flight path
    # plot arrows every 20 points to avoid cluttering
    for i in range(0, len(xs), 20):
        angle_rad = np.deg2rad(azs[i]) # convert azimuth angle from degrees to radians
        dx = np.cos(angle_rad) * arrow_length   # calculate the x component of the arrow
        dy = np.sin(angle_rad) * arrow_length   # calculate the y component of the arrow
        # draw the arrow at the current position
        plt.arrow(xs[i], ys[i], dx, dy, head_width=2.0, head_length=2.0, fc=color, ec=color, alpha=1.0) 

    ## Add final target markers
    tx, ty = final_targets.get(uid, (None, None))
    if tx is not None: # check if target exists for this UAV
        # add a cross at the target position
        plt.scatter(tx, ty, marker='+', s=100, color='black', linewidths=3) # marker for target position
        # add text label for the target
        plt.text(tx, ty-5, f"Target {uid}", fontsize=6, va='center', ha='center', color='black')

plt.title("UAV Flight Paths")
plt.xlabel("X Position (m)")
plt.ylabel("Y Position (m)")
plt.axis("equal") # equal scaling for x and y axes
plt.grid(True)  # add grid 
plt.legend(loc='upper left', fontsize=8)    # add explanation for each UAV (legend)
plt.tight_layout() # adjust layout to prevent overlap
plt.xlim(-100, 180)  # set x limits
plt.ylim(-100, 180) # set y limits
plt.show()  # display the plot
