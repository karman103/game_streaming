package main

import (
    "context"
    "github.com/docker/docker/api/types"
    "github.com/docker/docker/api/types/container"
    "github.com/docker/docker/client"
)

func StartGameContainer(sessionID string) (string, error) {
    cli, err := client.NewClientWithOpts(client.FromEnv)
    if err != nil {
        return "", err
    }

    resp, err := cli.ContainerCreate(context.Background(), &container.Config{
        Image: "inglebard/retroarch-web", // Example game image
        Tty:   true,
    }, nil, nil, nil, sessionID)

    if err != nil {
        return "", err
    }

    err = cli.ContainerStart(context.Background(), resp.ID, types.ContainerStartOptions{})
    if err != nil {
        return "", err
    }

    return resp.ID, nil
}

func StopGameContainer(containerID string) error {
    cli, _ := client.NewClientWithOpts(client.FromEnv)
    return cli.ContainerStop(context.Background(), containerID, nil)
}
