package com.example.lab5.activity;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.lab5.R;
import com.example.lab5.adapter.EpisodeList_Adapter;
import com.example.lab5.object.Episode;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class AddEpisodeActivity extends AppCompatActivity {

//    Written by:
//        Name              Zhenghao Li
//        Student netID     zhenghl3
//        StudentID         65553969

    TextView animeName;
    EditText editEpisodeName;
    EditText editEpisodeLink;
    EditText editEpisodeDescription;
    Button addEpisodeButton;

    ListView episode_listView;
    List<Episode> episodeList;

    DatabaseReference databaseReference;
    DatabaseReference anime_databaseReference;

    String category_id;
    String anime_id;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_episode);

        animeName = findViewById(R.id.animeName);
        editEpisodeName = findViewById(R.id.editEpisodeName);
        editEpisodeLink = findViewById(R.id.editEpisodeLink);
        editEpisodeDescription = findViewById(R.id.editEpisodeDescription);
        addEpisodeButton = findViewById(R.id.addEpisodeButton);

        episode_listView = findViewById(R.id.episode_listview);

        Intent intent = getIntent();

        episodeList = new ArrayList<>();

        anime_id = intent.getStringExtra(AddAnimeActivity.ANIME_ID);
        category_id = intent.getStringExtra(AddAnimeActivity.CATEGORY_ID);
        String name = intent.getStringExtra(AddAnimeActivity.ANIME_NAME);

        animeName.setText(name);

        databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("episodes").child(category_id).child(anime_id);
        anime_databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("anime").child(category_id).child(anime_id);

        addEpisodeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                saveEpisode();
            }
        });

        episode_listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
                Episode episode = episodeList.get(position);
                databaseReference.child(episode.getId()).removeValue();

                Toast.makeText(AddEpisodeActivity.this, "Successfully deleted "+episode.getName(), Toast.LENGTH_LONG).show();

                return true;
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                episodeList.clear();

                for (DataSnapshot episodeSnapShot : dataSnapshot.getChildren()){
                    Episode episode = episodeSnapShot.getValue(Episode.class);
                    episodeList.add(episode);
                }

                DatabaseReference curr_d = anime_databaseReference.child("count");
                curr_d.addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                        if (dataSnapshot.getValue() == null){
                            anime_databaseReference.removeValue();
                        } else {
                            anime_databaseReference.child("count").setValue(episodeList.size());
                        }
                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });

                Collections.sort(episodeList, new sortByName());

                EpisodeList_Adapter episodeList_adapter = new EpisodeList_Adapter(AddEpisodeActivity.this, episodeList);
                episode_listView.setAdapter(episodeList_adapter);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    private void saveEpisode(){
        String episode_name = editEpisodeName.getText().toString().trim();
        String episode_link = editEpisodeLink.getText().toString().trim();
        String episode_description = editEpisodeDescription.getText().toString().trim();

        if (!episode_name.isEmpty()){
            String id = databaseReference.push().getKey();

            Episode anime = new Episode(id, episode_name, episode_link, episode_description);

            databaseReference.child(id).setValue(anime);

            editEpisodeName.setText("");
            editEpisodeLink.setText("");
            editEpisodeDescription.setText("");

        } else {
            Toast.makeText(this,"Enter an episode name", Toast.LENGTH_LONG).show();
        }

    }

    class sortByName implements Comparator<Episode>{
        @Override
        public int compare(Episode o1, Episode o2) {
            return o1.getName().compareTo(o2.getName());
        }
    }
}
