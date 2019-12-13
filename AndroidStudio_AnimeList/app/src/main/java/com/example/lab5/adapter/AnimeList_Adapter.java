package com.example.lab5.adapter;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.example.lab5.R;
import com.example.lab5.object.Anime;

import java.util.List;

public class AnimeList_Adapter extends ArrayAdapter<Anime> {

//    Written by:
//        Name              Qirui Wu
//        Student netID     qiruiw3
//        StudentID         88471259

    private Activity context;
    private List<Anime> anime_list;

    public AnimeList_Adapter(Activity context, List<Anime> anime_list){
        super(context, R.layout.anime_layout, anime_list);

        this.context = context;
        this.anime_list = anime_list;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();

        View listViewItem = inflater.inflate(R.layout.anime_layout, null, true);

        TextView anime_name = listViewItem.findViewById(R.id.anime_name);
        TextView anime_rating = listViewItem.findViewById(R.id.anime_rating);
        TextView anime_episode_count = listViewItem.findViewById(R.id.anime_episode_count);

        Anime anime = anime_list.get(position);

        anime_name.setText(anime.getName());
        anime_rating.setText("Rating: " + anime.getRating());
        anime_episode_count.setText(String.valueOf(anime.getCount()) + " episodes stored");

        return listViewItem;
    }

}
