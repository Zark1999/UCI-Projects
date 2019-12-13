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
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.lab5.R;
import com.example.lab5.object.Anime;
import com.example.lab5.adapter.AnimeList_Adapter;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class AddAnimeActivity extends AppCompatActivity {

//    Written by:
//        Name              Qirui Wu
//        Student netID     qiruiw3
//        StudentID         88471259

    public static final String ANIME_NAME = "ANIME_NAME";
    public static final String ANIME_ID = "ANIME_ID";
    public static final String CATEGORY_ID = "CATAGORY_ID";

    TextView category_name;
    EditText edit_anime_text;
    SeekBar edit_anime_rating;
    Button add_anime_button;

    ListView anime_listView;
    List<Anime> animeList;

    int anime_rating;
    String cid;

    DatabaseReference databaseReference;
    DatabaseReference category_databaseReference;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_anime);

        category_name = findViewById(R.id.categoryName);
        edit_anime_text = findViewById(R.id.editAnimeName);
        edit_anime_rating = findViewById(R.id.editAnimeRating);
        add_anime_button = findViewById(R.id.addAnimeButton);

        anime_listView = findViewById(R.id.anime_listview);

        Intent intent = getIntent();

        animeList = new ArrayList<>();

        edit_anime_rating.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                anime_rating = progress;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });


        cid = intent.getStringExtra(MainActivity.CATEGORY_ID);
        String name = intent.getStringExtra(MainActivity.CATEGORY_NAME);

        category_name.setText(name);

        databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("anime").child(cid);
        category_databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("categories").child(cid);

        add_anime_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                saveAnime();
            }
        });

        anime_listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Anime anime = animeList.get(position);

                System.out.println(anime.getId());

                Intent intent = new Intent(AddAnimeActivity.this, AddEpisodeActivity.class);

                intent.putExtra(ANIME_ID, anime.getId());
                intent.putExtra(ANIME_NAME, anime.getName());
                intent.putExtra(CATEGORY_ID, cid);

                startActivity(intent);
            }
        });

        anime_listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
                Anime anime = animeList.get(position);

                DatabaseReference episode_databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("episodes").child(cid).child(anime.getId());
                episode_databaseReference.removeValue();

                databaseReference.child(anime.getId()).removeValue();

                Toast.makeText(AddAnimeActivity.this, "Successfully deleted "+anime.getName(), Toast.LENGTH_LONG).show();

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
                animeList.clear();

                for (DataSnapshot animeSnapShot : dataSnapshot.getChildren()){
                    Anime anime = animeSnapShot.getValue(Anime.class);
                    animeList.add(anime);
                }

                DatabaseReference curr_d = category_databaseReference.child("count");
                curr_d.addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                        if (dataSnapshot.getValue() == null){
                            category_databaseReference.removeValue();
                        } else {
                            category_databaseReference.child("count").setValue(animeList.size());
                        }
                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });

                Collections.sort(animeList, new sortByRating());

                AnimeList_Adapter animeList_adapter = new AnimeList_Adapter(AddAnimeActivity.this, animeList);
                anime_listView.setAdapter(animeList_adapter);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    private void saveAnime(){
        String anime_name = edit_anime_text.getText().toString().trim();

        if (!anime_name.isEmpty()){
            String id = databaseReference.push().getKey();

            Anime anime = new Anime(id, anime_name, anime_rating, 0);

            databaseReference.child(id).setValue(anime);

            edit_anime_text.setText("");
            edit_anime_rating.setProgress(0);

        } else {
            Toast.makeText(this,"Enter an anime", Toast.LENGTH_LONG).show();
        }
    }

    class sortByRating implements Comparator<Anime>{
        @Override
        public int compare(Anime o1, Anime o2) {
            return o2.getRating() - o1.getRating();
        }
    }
}
