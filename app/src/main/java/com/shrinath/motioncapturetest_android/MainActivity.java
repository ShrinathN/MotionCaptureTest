package com.shrinath.motioncapturetest_android;

import androidx.annotation.ColorInt;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.Buffer;
import java.sql.Struct;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    Handler handler;
    Thread thread;

    final static String TAG = "TAG";
    final static int PORT = 8080;

    final static float ACCEL_X_THRESHOLD = 10.0f;
    final static float ACCEL_Y_THRESHOLD = 10.0f;
    final static float ACCEL_Z_THRESHOLD = 10.0f;

    final static float GYRO_X_THRESHOLD = 5.0f;
    final static float GYRO_Y_THRESHOLD = 5.0f;
    final static float GYRO_Z_THRESHOLD = 5.0f;


    //UI widgets
    Button button_start;
    Button button_stop;
    TextView textView_accel_x;
    TextView textView_accel_y;
    TextView textView_accel_z;
    TextView textView_gyro_x;
    TextView textView_gyro_y;
    TextView textView_gyro_z;

    //sensors
    SensorManager sensorManager;
    Sensor accelerometer;
    Sensor gyroscope;


    public float accel_x;
    public float accel_y;
    public float accel_z;
    public float gyro_x;
    public float gyro_y;
    public float gyro_z;

    float map(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    Runnable updateUi = new Runnable() {
        @Override
        public void run() {
            int col_val;
            col_val = (int) map(Math.abs(accel_x), 0, ACCEL_X_THRESHOLD, 0.0f, 255.0f);
            textView_accel_x.setText(Float.toString(accel_x));
            textView_accel_x.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

            col_val = (int) map(Math.abs(accel_y), 0, ACCEL_Y_THRESHOLD, 0.0f, 255.0f);
            textView_accel_y.setText(Float.toString(accel_y));
            textView_accel_y.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

            col_val = (int) map(Math.abs(accel_z), 0, ACCEL_Z_THRESHOLD, 0.0f, 255.0f);
            textView_accel_z.setText(Float.toString(accel_z));
            textView_accel_z.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

            col_val = (int) map(Math.abs(gyro_x), 0, GYRO_X_THRESHOLD, 0.0f, 255.0f);
            textView_gyro_x.setText(Float.toString(gyro_x));
            textView_gyro_x.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

            col_val = (int) map(Math.abs(gyro_y), 0, GYRO_Y_THRESHOLD, 0.0f, 255.0f);
            textView_gyro_y.setText(Float.toString(gyro_y));
            textView_gyro_y.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

            col_val = (int) map(Math.abs(gyro_z), 0, GYRO_Z_THRESHOLD, 0.0f, 255.0f);
            textView_gyro_z.setText(Float.toString(gyro_z));
            textView_gyro_z.setBackgroundColor(Color.rgb(0 + col_val, 255 - col_val, 0));

        }
    };

    void startAccelerometerDataAcquisition() {
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);

        gyroscope = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        sensorManager.registerListener(this, gyroscope, SensorManager.SENSOR_DELAY_NORMAL);
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        thread = new Thread(new Runnable() {
            @Override
            public void run() {
                networkSenderLoop();
            }
        });

        startAccelerometerDataAcquisition();

        handler = new Handler();
        textView_accel_x = findViewById(R.id.textView_accel_x);
        textView_accel_y = findViewById(R.id.textView_accel_y);
        textView_accel_z = findViewById(R.id.textView_accel_z);

        textView_gyro_x = findViewById(R.id.textView_gyro_x);
        textView_gyro_y = findViewById(R.id.textView_gyro_y);
        textView_gyro_z = findViewById(R.id.textView_gyro_z);

        button_start = findViewById(R.id.button_start);
        button_stop = findViewById(R.id.button_stop);
        button_stop.setEnabled(false);

        button_start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                thread.start();
                button_start.setEnabled(false);
                button_stop.setEnabled(true);
            }
        });

        button_stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                thread.interrupt();
                thread.stop();
                button_stop.setEnabled(false);
                button_start.setEnabled(true);
            }
        });

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor sensor = event.sensor;
        if (sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
//            Log.d(TAG, "x : " + Float.toString(x) + "\ny : " + Float.toString(y) + "\nz : " + Float.toString(z) + "\n");
            accel_x = event.values[0];
            accel_y = event.values[1];
            accel_z = event.values[2];
            handler.post(updateUi);
        } else if (sensor.getType() == Sensor.TYPE_GYROSCOPE) {
            gyro_x = event.values[0];
            gyro_y = event.values[1];
            gyro_z = event.values[2];
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public void networkSenderLoop() {
        byte data[] = new byte[24];
        int temp;
        DatagramSocket datagramSocket;
        try {
            datagramSocket = new DatagramSocket(PORT);
            while (!Thread.interrupted()) {
                //accelerometer
                temp = Float.floatToIntBits(accel_x);
                data[0] = (byte) ((temp & 0x000000ff) >> 0);
                data[1] = (byte) ((temp & 0x0000ff00) >> 8);
                data[2] = (byte) ((temp & 0x00ff0000) >> 16);
                data[3] = (byte) ((temp & 0xff000000) >> 24);
                temp = Float.floatToIntBits(accel_y);
                data[4] = (byte) ((temp & 0x000000ff) >> 0);
                data[5] = (byte) ((temp & 0x0000ff00) >> 8);
                data[6] = (byte) ((temp & 0x00ff0000) >> 16);
                data[7] = (byte) ((temp & 0xff000000) >> 24);
                temp = Float.floatToIntBits(accel_z);
                data[8] = (byte) ((temp & 0x000000ff) >> 0);
                data[9] = (byte) ((temp & 0x0000ff00) >> 8);
                data[10] = (byte) ((temp & 0x00ff0000) >> 16);
                data[11] = (byte) ((temp & 0xff000000) >> 24);

                //gyroscope
                temp = Float.floatToIntBits(gyro_x);
                data[12] = (byte) ((temp & 0x000000ff) >> 0);
                data[13] = (byte) ((temp & 0x0000ff00) >> 8);
                data[14] = (byte) ((temp & 0x00ff0000) >> 16);
                data[15] = (byte) ((temp & 0xff000000) >> 24);
                temp = Float.floatToIntBits(gyro_y);
                data[16] = (byte) ((temp & 0x000000ff) >> 0);
                data[17] = (byte) ((temp & 0x0000ff00) >> 8);
                data[18] = (byte) ((temp & 0x00ff0000) >> 16);
                data[19] = (byte) ((temp & 0xff000000) >> 24);
                temp = Float.floatToIntBits(gyro_z);
                data[20] = (byte) ((temp & 0x000000ff) >> 0);
                data[21] = (byte) ((temp & 0x0000ff00) >> 8);
                data[22] = (byte) ((temp & 0x00ff0000) >> 16);
                data[23] = (byte) ((temp & 0xff000000) >> 24);


                DatagramPacket datagramPacket = new DatagramPacket(data, 24, InetAddress.getByName("192.168.43.28"), PORT);
                datagramSocket.send(datagramPacket);
            }
        } catch (Exception e) {
            Log.d(TAG, "networkSenderLoop: " + e);
        }
    }
}