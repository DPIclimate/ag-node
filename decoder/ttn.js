// TODO handle daylight savings
function unix_to_AU_datetime(unixTimeStamp) {
	localUnixTime = (unixTimeStamp + (3600 * 10)) * 1000; // UTC+10:00
	let date = new Date(localUnixTime);
	return ('0' + date.getDate()).slice(-2) + '/' + ('0' + (date.getMonth() + 1)).slice(-2) + '/' + date.getFullYear() + ' ' + ('0' + date.getHours()).slice(-2) + ':' + ('0' + date.getMinutes()).slice(-2);
}


function bytes_to_hex(byteArray) {
	return Array.from(byteArray, function(byte) {
		return ('0' + (byte & 0xFF).toString(16)).slice(-2);
	}).join('')
}


function decodeUplink(input) {
	var payloadType = input.bytes[0];
	if(input.fPort === 1){
		var unixTime = input.bytes[1] | (input.bytes[2] << 8) | (input.bytes[3] << 16) | (input.bytes[4] << 24);
		var dateTime = unix_to_AU_datetime(unixTime);
		var scaleID = input.bytes[5];
		var startWeight = (input.bytes[6] | (input.bytes[7] << 8)) / 100.0;
		var middleWeight = (input.bytes[8] | (input.bytes[9] << 8)) / 100.0;
		var endWeight = (input.bytes[10] | (input.bytes[11] << 8)) / 100.0;
		var avWeight = (input.bytes[12] | (input.bytes[13] << 8)) / 100.0;
		var deltaWeight = (input.bytes[14] | (input.bytes[15] << 24 >> 16)) / 100.0;
		var timeOnScale = (input.bytes[16] | (input.bytes[17] << 8) | (input.bytes[18] << 16) | (input.bytes[19] << 24)) / 1000.0;
		var stdevWeight = (input.bytes[20] | (input.bytes[21] << 8)) / 100.0;
		var estimatedWeight = (input.bytes[22] | (input.bytes[23] << 8)) / 100.0;
		var estWeightFlag = input.bytes[24];
		var maxWeight = (input.bytes[25] | (input.bytes[26] << 8)) / 100.0;
		var maxStableWeight = (input.bytes[27] | (input.bytes[28] << 8) / 100.0);
		var stableTime = (input.bytes[29] | (input.bytes[30] << 8) | (input.bytes[31] << 16) | (input.bytes[32] << 24))  / 1000.0;
		var rmse = (input.bytes[33] | (input.bytes[34] << 8) / 100.0);
		return {
			data: {
				"bytes": input.bytes,
				"HEX": bytes_to_hex(input.bytes),
				"UNIX_time": unixTime,
				"Local_time": dateTime,
				"Scale_ID": scaleID,
				"Start_weight": startWeight,
				"Middle_weight": middleWeight,
				"End_weight": endWeight,
				"Average_weight": avWeight,
				"Delta_weight": deltaWeight,
				"Drink_time": timeOnScale,
				"Stdev_weight": stdevWeight,
				"Estimated_weight": estimatedWeight,
				"Estimated_weight_flag": estWeightFlag,
				"Max_weight": maxWeight,
				"Max_stable_weight": maxStableWeight,
				"Stable_time": stableTime,
				"RMSE": rmse
			},
			warnings: [],
			errors: []
		};
	}
	else if(input.fPort === 2){
		unixTime = input.bytes[1] | (input.bytes[2] << 8) | (input.bytes[3] << 16) | (input.bytes[4] << 24);
		dateTime = unix_to_AU_datetime(unixTime);
		var batteryV = (input.bytes[5] | (input.bytes[6] << 8)) / 100.0;
		var batteryA = (input.bytes[7] | (input.bytes[8] << 8)) / 100.0;
		var batteryW = (input.bytes[9] | (input.bytes[10] << 8)) / 100.0;
		var solarV = (input.bytes[11] | (input.bytes[12] << 8)) / 100.0;
		var solarA = (input.bytes[13] | (input.bytes[14] << 8)) / 100.0;
		var solarW = (input.bytes[15] | (input.bytes[16] << 8)) / 100.0;
		var temperature = (input.bytes[17] | (input.bytes[18] << 24 >> 16)) / 100.0;
		return {
			data: {
				"bytes": input.bytes,
				"HEX": bytes_to_hex(input.bytes),
				"UNIX_time": unixTime,
				"Local_time": dateTime,
				"Battery (V)": batteryV,
				"Battery (A)": batteryA,
				"Battery (W)": batteryW,
				"Solar (V)": solarV,
				"Solar (A)": solarA,
				"Solar (W)": solarW,
				"Temperature": temperature
			},
			warnings: [],
			errors: []
		};
	}
	else if (input.fPort === 3) {
		return {
			data: {
				"Connection": "Success"
			},
			warnings: [],
			errors: []
		};
	}
	else {
		return {
			data: {
				"Unknown": "fPort cannot be identified"
			},
			warnings: [],
			errors: []
		};
	}
}
