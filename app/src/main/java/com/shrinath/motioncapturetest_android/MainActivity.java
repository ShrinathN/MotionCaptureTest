package com.shrinath.motioncapturetest_android;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
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

import java.io.DataOutputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    Handler handler;

    final static String TAG = "TAG";
    final static int PORT = 8080;

    Button button_start;
    TextView textView_x;
    TextView textView_y;
    TextView textView_z;
    TextView textView_x_label;
    TextView textView_y_label;
    TextView textView_z_label;

    SensorManager sensorManager;
    Sensor accelerometer;


    public float z;
    public float x;
    public float y;

    Runnable updateUi = new Runnable() {
        @Override
        public void run() {
            textView_x.setText(Float.toString(x));
            textView_y.setText(Float.toString(y));
            textView_z.setText(Float.toString(z));
        }
    };

    void startAccelerometerDataAcquisition() {
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        handler = new Handler();
        textView_x_label = findViewById(R.id.textView_x_label);
        textView_y_label = findViewById(R.id.textView_y_label);
        textView_z_label = findViewById(R.id.textView_z_label);
        textView_x = findViewById(R.id.textView_x);
        textView_y = findViewById(R.id.textView_y);
        textView_z = findViewById(R.id.textView_z);

        button_start = findViewById(R.id.button_start);
        button_start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startAccelerometerDataAcquisition();
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        networkSenderLoop();
                    }
                }).start();
                button_start.setEnabled(false);
            }
        });

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor sensor = event.sensor;
        if(sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
//            Log.d(TAG, "x : " + Float.toString(x) + "\ny : " + Float.toString(y) + "\nz : " + Float.toString(z) + "\n");
            x = event.values[0];
            y = event.values[1];
            z = event.values[2];
            handler.post(updateUi);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public void networkSenderLoop() {
        try {
            ServerSocket serverSocket = new ServerSocket(PORT);
            Log.d(TAG, "networkSenderLoop: Listening!! " + InetAddress.getLocalHost());
            Socket socket = serverSocket.accept();
            Log.d(TAG, "networkSenderLoop: Connected!");
            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
            while(socket.isConnected() && !socket.isClosed()) {
                dataOutputStream.writeFloat(x);
                dataOutputStream.writeFloat(y);
                dataOutputStream.writeFloat(z);
                Thread.sleep(33);
            }
        }catch (Exception e){
            Log.d(TAG, "networkSenderLoop: " + e);
        }
    }
}