clear all; close all; clc;
pose = load('pose.txt');
P = [];
for j = 1:8
    new_point = zeros(361,2);
    name = 'log'+string(j)+'.txt';
    log = load(name);
    p = pose(j,:);
    for i = 1:361
        new_point(i,1) = (log(i,1)+0.26)*cos(p(3))-log(i,2)*sin(p(3))+p(1);
        new_point(i,2) = (log(i,1)+0.26)*sin(p(3))+log(i,2)*cos(p(3))+p(2);
        P = [new_point;P];
    end
end
%scatter(log5(:,1), log5(:,2));
scatter(P(:,1), P(:,2));
%xlim([1,3]);
%ylim([1,2]);