package com.example.esp_mqtt;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;
import android.os.TokenWatcher;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {
    private ImageView light1;
    private TextView temp1;
    private TextView humi1;
    private TextView ITflag;
    private Button login1;

    private EditText ed1;

    private int led_flag =1;
    /////////MQTT//////////////////////////////////////////
    private String host = "tcp://IP:1883"; //IP：輸入服務器地址或者IP
    private String userName = "android";
    private String passWord = "android";
    private String mqtt_id = "123456"; //定义成自己的QQ号  切记！不然会掉线！！！
    private String mqtt_sub_topic = "my_esp_PUS"; //为了保证你不受到别人的消息  哈哈
    private String mqtt_pub_topic = "my_esp_SUB"; //为了保证你不受到别人的消息  哈哈  自己QQ好后面加 _PC
    private ScheduledExecutorService scheduler;
    private MqttClient client;
    private MqttConnectOptions options;
    private Handler handler;

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//*****************ID**********************//
        temp1 = findViewById(R.id.temp1);
        humi1 = findViewById(R.id.humi1);
        ITflag = findViewById(R.id.ITflag);
        light1 = findViewById(R.id.light1);
        ed1 = findViewById(R.id.ed1);
        login1 = findViewById(R.id.login1);
        //*************************************//
        light1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Toast.makeText(MainActivity.this,"hello",Toast.LENGTH_SHORT).show();
                if(led_flag == 0)
                {
                    publishmessageplus(mqtt_pub_topic,"{\"LED_SW\":1}");
                    led_flag = 1;
                }else
                {
                    publishmessageplus(mqtt_pub_topic,"{\"LED_SW\":0}");
                    led_flag = 0;
                }
            }
        });
        login1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mqtt_init();
                startReconnect();
            }
        });





        handler = new Handler() {
            @SuppressLint("SetTextI18n")
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //开机校验更新回传
                        break;
                    case 2:  // 反馈回传

                        break;
                    case 3:  //MQTT 收到消息回传   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        //String T_val = msg.obj.toString().substring(msg.obj.toString().indexOf("temperature\":")+13,msg.obj.toString().indexOf("}"));
                        //String T_val0 = msg.obj.toString().substring(msg.obj.toString().indexOf("---"));
                        String T_val1 = msg.obj.toString().substring(msg.obj.toString().indexOf("Temp\":")+6,msg.obj.toString().indexOf(","));
                        String T_val2 = msg.obj.toString().substring(msg.obj.toString().indexOf("hum\":")+5,msg.obj.toString().indexOf("}"));
                       //
                        // Toast.makeText(MainActivity.this,T_val2,Toast.LENGTH_SHORT).show();
                        //String text_val2 = T_val2;
                        temp1.setText(T_val1);
                        humi1.setText(T_val2);

                        //Toast.makeText(MainActivity.this,T_val1 ,Toast.LENGTH_SHORT).show();
                        //text_test.setText(msg.obj.toString());
                        break;
                    case 30:  //连接失败
                        Toast.makeText(MainActivity.this,"连接失败" ,Toast.LENGTH_SHORT).show();
                        ITflag.setText("连接失败");
                        break;
                    case 31:   //连接成功
                        Toast.makeText(MainActivity.this,"连接成功" ,Toast.LENGTH_SHORT).show();
                        ITflag.setText("连接成功");
                        try {
                            client.subscribe(mqtt_sub_topic,0);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };
    }

    //////////////////////////////////////////////////////////////////////
    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host, mqtt_id,
                    new MemoryPersistence());
            //MQTT的连接设置
            options = new MqttConnectOptions();
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------"
                            + token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
                    msg.obj = topicName + "---" + message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if(!(client.isConnected()) )  //如果还未连接
                    {
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    }
    private void startReconnect() {
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected()) {
                    Mqtt_connect();
                }
            }
        }, 0 * 1000, 10 * 1000, TimeUnit.MILLISECONDS);
    }
    private void publishmessageplus(String topic,String message2)
    {
        if (client == null || !client.isConnected()) {
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {

            e.printStackTrace();
        }
    }
}