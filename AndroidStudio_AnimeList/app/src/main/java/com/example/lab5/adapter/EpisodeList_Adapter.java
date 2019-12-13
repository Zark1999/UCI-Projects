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
import com.example.lab5.object.Episode;

import java.util.List;

public class EpisodeList_Adapter extends ArrayAdapter<Episode> {

//    Written by:
//        Name              Zhenghao Li
//        Student netID     zhenghl3
//        StudentID         65553969

    private Activity context;
    private List<Episode> episodeList;

    public EpisodeList_Adapter(Activity context, List<Episode> episodeList){
        super(context, R.layout.episode_layout, episodeList);

        this.context = context;
        this.episodeList = episodeList;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();

        View listViewItem = inflater.inflate(R.layout.episode_layout, null, true);

        TextView episode_number = listViewItem.findViewById(R.id.episode_name);
        TextView episode_link = listViewItem.findViewById(R.id.episode_link);
        TextView episode_description = listViewItem.findViewById(R.id.episode_description);

        Episode episode = episodeList.get(position);

        episode_number.setText(String.valueOf(episode.getName()));
        episode_link.setText("Link: " + episode.getLink());
        episode_description.setText("Description: " + episode.getDescription());

        return listViewItem;
    }
}
