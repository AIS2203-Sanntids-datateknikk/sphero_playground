import qwiic


def change_address(old_address: int, new_address):

	# Connect to Mux
	mux = qwiic.QwiicTCA9548A()
	# Initially Disable All Channels
	mux.disable_all()

	print("Running VL53L1X I2C Address Change")

	# Scans I2C addresses
	avail_addresses = qwiic.scan()

	# Check I2C addresses for Pi Servo pHat
	while 0x40 in avail_addresses:
		if 0x70 in avail_addresses:

			pca = qwiic.QwiicPCA9685()
			pca.set_addr_bit(0, 0)

			avail_addresses = qwiic.scan()

		# Remove Pi Servo pHat from avail_address list
		try:
			avail_addresses.remove(0x40)
		except ValueError:
			print("Addresses for Pi Servo pHat (0x40) not in scanned addresses.")

	while 0x29 not in avail_addresses:
		print("VL53L1X ToF sensor not detected on I2C bus at default address (0x29 or 41).")
		mux.list_channels()

		mux.enable_channels(4)

		avail_addresses = qwiic.scan()

	print("Initializing Device")
	ToF = qwiic.QwiicVL53L1X(old_address)
	ToF.SensorInit()
	ToF.SetI2CAddress(new_address)
	print("Address change to new address (", hex(new_address), ") is complete.")
